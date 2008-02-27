/*
 * libmagic -  automagically determine file type
 * $Id: magic.c,v 1.41 2000/05/26 07:31:31 fyre Exp $
 *
 * Copyright (c) 2000 Tim Robbins 		<tim_r@hotmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

/*
 * Determine file type by reading a magic(4) database and performing the
 * tests on a file or buffer.
 */

#ifdef __GNUC__
__attribute__((unused))
#endif 
static char rcsid[] = "$Id: magic.c,v 1.41 2000/05/26 07:31:31 fyre Exp $";

/* needed for endianness and snprintf on glibc */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

#include "sysdep.h"
#define MAGIC_C
#include "magic.h"
#undef MAGIC_C

/* exported functions */
magichandle_t* magic_init (unsigned flags);
int magic_close (magichandle_t* mh);
int magic_read_entries (magichandle_t* mh, const char* fn);
const char* magic_identify_file (magichandle_t* mh, const char* fn);
const char* magic_identify_buffer (magichandle_t* mh, const uint8_t* buf,
        size_t buflen);
int magic_add_entry_string (magichandle_t* mh, const char* ent);

/* private functions */
static const char* read_number (const char* buf, int* n);
static const char* read_number_u (const char* buf, unsigned int* n);
static const char* read_string (const char* buf, uint8_t** s,
	size_t* stlen, int stopspaces, int zeroterm);
static const char* read_indirect_offset (const char* buf, 
	magicentry_t* me);
static const char* read_offset (const char* buf, magicentry_t* me);
static const char* read_type (const char* buf, magicentry_t* me);
static const char* read_comparison (const char* buf, magicentry_t* me);
static const char* read_level (const char* buf, magicentry_t* me);
static const char* read_mask (const char* buf, magicentry_t* me);
static const char* read_test_value (const char* buf, magicentry_t *me);
static int parse_line (const char* buf, magicentry_t *me);
static uint32_t byteswap_4 (uint32_t* n);
static uint16_t byteswap_2 (uint16_t* n);
static int process_test (magichandle_t* mh, unsigned int level, unsigned int i,
	int offset, const uint8_t* data, size_t len, char* desc);
static int do_test (const magicentry_t* me, unsigned int prevoffset,	
	const uint8_t* data, int len, char* desc);
static int calculate_offset (const magicentry_t* me, unsigned int offset,
	const uint8_t* data, unsigned int len);
static int do_string_test (const magicentry_t* me, unsigned int ofs,
	const uint8_t* data, unsigned int len, char* desc);
static int do_integer_test (const magicentry_t* me, unsigned int ofs,
	const uint8_t* data, unsigned int len, char* desc);
static int validate_format (const char* format, type_t type);
static int snprintf1s (char* dest, size_t n, const char* format, 
	const char* str);
static int snprintf1i (char* dest, size_t n, const char* format, 
	unsigned int i);
static void free_entries (magichandle_t* mh);
static void self_test (void);
static void rescale_level_zero (magichandle_t* mh);
static int process_test_level_zero (magichandle_t* mh, unsigned int i,
	int offset, const uint8_t* data, size_t len, char* desc);
static int process_test_level (magichandle_t*mh, unsigned int level, 
	unsigned int i, int offset, const uint8_t* data, size_t len, 
	char* desc);
static void reorder_level_zero (magichandle_t* mh, unsigned int changed);

/*
 * magic_init
 *
 * Allocate and initialise a magichandle_t. This handle is then passed
 * to all externally visible routines. Must be freed with magic_close
 * (don't even think about using free()!).
 *
 * Returns the new handle or NULL if there was not enough memory.
 */
magichandle_t* magic_init (unsigned flags)
{
	magichandle_t* mh;

	self_test();
	
	mh = malloc(sizeof(magichandle_t));

	if (!mh)
		return NULL;

	mh->flags = flags;
	mh->numentries = 0;
	mh->num_l0_ents = 0;
	mh->l0_ents = NULL;
	mh->mentries = NULL;
	*mh->desc = '\0';

	return mh;
}

/*
 * magic_close
 *
 * Free a magichandle_t and its dynamically allocated contents.
 *
 * Returns nonzero on success, zero on error.
 */
int magic_close (magichandle_t* mh)
{
	if (!mh)
		/* invalid handle or already freed */
		return 0;
	else {
		if (mh->mentries)
			free_entries(mh);
			
		if (mh->l0_ents) { 
			free(mh->l0_ents);
			mh->l0_ents = NULL;
		}
			
		free(mh);
		mh = NULL;
		
		return 1;
	}
}

/*
 * magic_identify_buffer
 *
 * Try to determine the type of a file when given a buffer (`buf') containing
 * the first `buflen' bytes of the file.
 *
 * Returns a string describing the file if it could be identified via the
 * the currently loaded magic database, or NULL if it could not be.
 */
const char* magic_identify_buffer (magichandle_t* mh, const uint8_t* buf, 
	size_t buflen)
{
	
	if ((!mh) || (!buf) || (buflen == 0))
		return NULL;

	*mh->desc = '\0';

	/* start traversing the `tree' at the start (entry 0) */
	if ((!process_test (mh, 0, 0, 0, buf, buflen, mh->desc)) 
		|| (!*mh->desc))
		return NULL;

	return mh->desc;
}


/*
 * magic_identify_file
 *
 * Try to determine the type of a given file, returns file type if successful,
 * otherwise NULL.
 *
 * Returns a string describing the file `fn' if it could be identified via
 * the currently loaded magic database, or NULL if it could not be.
 */
const char* magic_identify_file (magichandle_t* mh, const char* fn)
{
	uint8_t* buf;
	size_t buflen;
	FILE* fp;
	const char* desc;

	if ((!fn) || (!mh)) 
		return NULL;
	
	/* allocate space for the first HOWMANY bytes of the file and read
	   the first HOWMANY bytes from the file if possible */
	
	if ((buf = malloc(HOWMANY)) == NULL)
		return NULL;

	if ((fp = fopen(fn,"rb")) == NULL) 
		return NULL;
	
	buflen = fread(buf, 1, HOWMANY, fp);
	fclose(fp);

	/* resize the buffer so we don't waste memory */
	if ((buf = realloc(buf, buflen)) == NULL)
		return NULL;

	/* try to identify the buffer */
	desc = magic_identify_buffer (mh, buf, buflen);

	free (buf);

	return desc;
}

/*
 * magic_read_entries
 *
 * Adds the entries from the specified file `fn' to the table.
 *
 * Returns nonzero on success, zero on failure.
 */
int magic_read_entries (magichandle_t* mh, const char* fn)
{
	FILE* fp;
	char buf[MAXLINE];
	char *nl;
	size_t allocentries, alloc_l0; /* keep track of how many entries
					  we've allocated */
	size_t linenum = 0;
	
	if ((!mh) || (!fn))
		return 0;

	allocentries = mh->numentries;
	alloc_l0 = mh->num_l0_ents;
	
	fp = fopen(fn,"r");
	if (!fp) return 0;
	
	/* read at most MAXENRIES lines from the magic file */
	while ((fgets(buf,sizeof(buf),fp)) && (mh->numentries < MAXENTRIES)) {
		linenum++;

		/* do we need to grow the array? */
		if ((allocentries - mh->numentries) == 0) {
			allocentries += GROWBY;
			mh->mentries = realloc(mh->mentries,
				sizeof(magicentry_t)*allocentries);
			if (mh->mentries == NULL) 
				return 0;
		}
		
		/* remove trailing newline */
		if ((nl = strrchr(buf,'\n'))) 
			*nl = '\0';
		else {
			/* no newline character found */

			int c;
			
			/* line is not newline terminated */
			if (strlen(buf) != sizeof(buf)-1)
				return 1;
			
			/* line is too long, eat all characters up to the next
			   newline */
			while (c=fgetc(fp), ((c != '\n') && (c != EOF))) ;
			
			if (c == EOF) return 1;
			
			continue;
		}
		
		/* try to parse it, ignore errors (and comments) */
		if (!parse_line (buf,&mh->mentries[mh->numentries]))
			continue;

		/* if it's a level 0 entry, add it to the end of the specia 
		   level 0 list */
		if (mh->mentries[mh->numentries].level == 0) {
			/* grow the array */
			if ((alloc_l0 - mh->num_l0_ents) == 0) {
				alloc_l0 += GROWBY;
				mh->l0_ents = realloc(mh->l0_ents, 
					sizeof(size_t) * alloc_l0);
				if (!mh->l0_ents)
					return 0;
			}

			/* and add to the end of the list */
			mh->l0_ents[mh->num_l0_ents++] = mh->numentries;
		}
			
		mh->numentries++;
	}
	
	fclose(fp);

	/* free up wasted entries */
	allocentries = mh->numentries;
	mh->mentries = realloc (mh->mentries, sizeof(magicentry_t)*allocentries);
	if (mh->mentries == NULL)
		return 0;	

	/* free up wasted level 0 entries */
	alloc_l0 = mh->num_l0_ents;
	mh->l0_ents = realloc (mh->l0_ents, sizeof(size_t)*alloc_l0);
	if (mh->l0_ents == NULL)
		return 0;
	
	return 1;
}

/*
 * magic_add_entry_string
 *
 * Add an entry to the table from a string.
 *
 * Returns zero on failure, nonzero on success.
 */
int magic_add_entry_string (magichandle_t* mh, const char* ent)
{
	if ((!mh) || (!ent))
		return 0;
	
	/* grow the array */
	mh->numentries++;
	mh->mentries = realloc (mh->mentries, sizeof(magicentry_t)*mh->numentries);
	if (mh->mentries == NULL)
		return 0;

	/* attempt to parse the line they gave us */
	if (!parse_line (ent, &mh->mentries[mh->numentries-1])) {
		/* ... parse failed; free up the memory we allocated for it */
		mh->numentries--;
		mh->mentries = realloc (mh->mentries, sizeof(magicentry_t)*mh->numentries);

		if (mh->mentries == NULL)
			return 0;	

		return 0;
	}

	return 1;
}

/* end externally visible functions */

/*
 * self_test
 *
 * Check that the runtime environment is what we expected (byte order, 
 * integer sizes, etc.).
 */
static void self_test (void) 
{
	union {
		uint32_t l;
		uint8_t b[4];
	} z;
	
	/* integer sizes */
	assert(sizeof(uint8_t) == 1);
	assert(sizeof(int8_t) == 1);
	assert(sizeof(uint16_t) == 2);
	assert(sizeof(int16_t) == 2);
	assert(sizeof(uint32_t) == 4);
	assert(sizeof(int32_t) == 4);

	/* endianness */
	z.l = 0x12345678;

#if BYTE_ORDER == LITTLE_ENDIAN
	assert (z.b[0] == (uint8_t)0x78);
#elif BYTE_ORDER == BIG_ENDIAN
	assert (z.b[0] == (uint8_t)0x12);
#else
	#error "I don't know how to deal with your endianness"
#endif
	
}

/*
 * free_entries
 *
 * Free all of the magic entries associated with the handle.
 */
static void free_entries (magichandle_t* mh)
{
	unsigned int i;
	
	assert(mh);
	assert(mh->mentries);
	
	for (i=0; i<mh->numentries; i++) {
		if (mh->mentries[i].msg) {
			free(mh->mentries[i].msg);
			mh->mentries[i].msg = NULL;
		}
		if (mh->mentries[i].str.value) {
			free(mh->mentries[i].str.value);
			mh->mentries[i].str.value = NULL;
		}
	}
	
	free(mh->mentries);
	mh->mentries = NULL;
	mh->numentries = 0;
}

/*
 * snprintf1s
 *
 * Wrapper around printf to print one string safely.
 *
 * Returns zero if the format was invalid otherwise returns the result
 * of snprintf.
 */
static int snprintf1s (char* dest, size_t n, const char* format, const char* str)
{
	assert(dest);
	assert(format);
	
	if (!validate_format (format, string_type))
		return 0;

	return snprintf(dest,n,format, str);
}

/*
 * snprintf1i
 *
 * Wrapper around printf to print one integer safely.
 *
 * Returns zero if the format was invalid otherwise returns the result
 * of snprintf.
 */
static int snprintf1i (char* dest, size_t n, const char* format, unsigned int i)
{
	assert(dest);
	assert(format);
	
	if (!validate_format (format, long_type))
		return 0;

	return snprintf(dest,n,format,i);
}

/*
 * validate_format
 *
 * Make sure a printf format string is safe to use; trying to print an integer
 * as a string, etc. can be disasterous.
 *
 * Also checks to make sure only one conversion exists.
 *
 * Returns zero if unsafe to use, nonzero on if safe.
 */
static int validate_format (const char* format, type_t type)
{
	const char *c;
	unsigned percents = 0;
	unsigned found = 0; /* number of valid, recognised conversions */

	assert(format);

	for (c = format; *c; c++) {
		/* beginning of a conversion */
		if (*c == '%') {
			
			if (!*++c) break;
			if (*c == '%') break;
			
			if (++percents > 1)
				return 0;

			/* skip over the flags */
			while ((*c) && (strchr("%#0- '",*c))) c++; 
			if (!*c) break;
			
			/* skip over field width */
			while (isdigit(*c)) c++;
			if (!*c) break;

			/* skip over precision */
			if (*c == '.') {
				if (!*++c) break;
				while (isdigit(*c)) c++;
				if (!*c) break;
			}

			/* skip over other modifiers */
			while ((*c) && (strchr("hlLqZ",*c))) c++;
			if (!*c) break;
			
			switch (*c) {
				/* integer */
				case 'd': case 'i': case 'o':
				case 'u': case 'x': case 'X':
				case 'c':
					if ((type == byte_type) ||
						(type == short_type) ||
						(type == long_type))
						found++;

					break;
					
				/* string */
				case 's': 
					if (type == string_type)
						found++;

					break;
			}
		}
	}

	return (percents == found) ? 1 : 0;
}

/*
 * read_number
 *
 * Read a C-style number from the buffer; hexadecimal numbers are prefixed
 * with "0x" (eg. 0xF3), octal numbers are prefixed with "0" (eg. 0363),
 * and anything else is assumed to be decimal. 
 *
 * Returns a pointer to the next character following the number.
 */
static const char* read_number (const char* buf, int* n)
{
	long tmp;
	char* newpos;
	
	assert(buf);
	assert(n);
	
	if (((tmp = strtol(buf,&newpos,0)) == LONG_MAX)
		|| (tmp == LONG_MIN))

		return NULL;
	
	buf = newpos;
	*n = (int)tmp;
			
	return buf;
}

/*
 * read_number_u
 *
 * Same as read_number but is unsigned.
 */
static const char* read_number_u (const char* buf, unsigned int* n)
{
	unsigned long tmp;
	char* newpos;
	
	assert(buf);
	assert(n);
	
	if ((tmp = strtoul(buf,(char**)&newpos,0)) == ULONG_MAX)
		return NULL;

	buf = newpos;
	*n = (unsigned int)tmp;
	
	return buf;
}

/*
 * read_string
 *
 * Allocate and read a string from the buffer, interpreting C-style backslash 
 * escapes sequences.
 *
 * Returns the place in the buffer where the reading of the string stopped, or
 * NULL if an invalid buffer (`buf') was passed or if there was not enough
 * memory to for the string. 
 */
static const char* read_string (const char* buf, uint8_t** s,
	size_t* stlen, int stopspaces, int zeroterm)
{
	uint8_t* sb;
	uint8_t* sbp;
	
	assert(buf);
	assert(s);

	/* allocate some memory for the string we're about to read */
	if ((sb = malloc(MAXSTRING)) == NULL)
		return NULL;
	
	sbp = sb;

	/* skip leading spaces */
	while ((*buf) && isspace(*buf)) buf++;
	if (!*buf) {
		free(sb);
		return NULL;
	}
	
	/* read at most MAXSTRING characters, possibly leaving room for
	   the terminating nul */
	for (; ((sbp-sb)<(MAXSTRING-(zeroterm?1:0))) && (*buf);) {
		if ((isspace(*buf)) && (stopspaces)) break;
		
		/* translate C backslash escape sequences */
		if (*buf == '\\') {
			unsigned char newchar;
			
			buf++;
			if (*buf == '\\') {
				newchar = '\\';
				buf++;
			} else if (*buf == 'n') {
				newchar = '\n';
				buf++;
			} else if (*buf == 'r') {
				newchar = '\r';
				buf++;
			} else if (*buf == 'v') {
				newchar = '\v';
				buf++;
			} else if (*buf == 'b') {
				newchar = '\b';
				buf++;
			} else if (*buf == 'f') {
				newchar = '\f';
				buf++;
			} else if (*buf == 't') {
				newchar = '\t';
				buf++;
			} else if ((isdigit(*buf)) && (*buf != '9') && (*buf != '8')) {
				/* octal character (\NNN) */
				char* newpos;
				char numbuf[4]; /* 3 digits + terminating null */
				
				strncpy (numbuf, buf, 3);
				newchar = (unsigned char)strtoul(numbuf,&newpos,8);
				buf += newpos - numbuf;
			} else if (*buf == 'x') {
				/* hexadecimal character (\xNN) */
				char* newpos;
				char numbuf[3]; /* 2 digits + null */
				
				++buf;
				strncpy (numbuf, buf, 2);
				newchar = (unsigned char)strtoul(numbuf,&newpos,16);
				buf += newpos - numbuf;
			} else newchar = *(buf++);

			*(sbp++) = newchar;
		} else *(sbp++) = *(buf++); /* just copy normal characters */
	}

	if (sbp == sb) { /* zero length string */
		*s = NULL;
		if (stlen) *stlen = 0;
		free (sb);

		return buf;
	} else { /* shrink string and return it */
		int slen;

		if (zeroterm) *sbp++ = '\0';

		slen = sbp-sb;
		
		if ((sb = realloc (sb, slen)) == NULL)
			return NULL;

		*s = sb;
		
		if (stlen) *stlen = slen;
	}
	
	return buf;
}

/*
 * read_indirect_offset
 *
 * Read an indirect offset.
 *
 * Returns the place in the buffer where the reading finished, or NULL
 * if an error occurred.
 */
static const char* read_indirect_offset (const char* buf, magicentry_t* me)
{
	const char* b2;
	
	assert(buf);
	assert(me);

	me->indirect = 1;
	
	if (!(buf = read_number_u(buf,&me->offset))) return NULL;
	if (*buf == '.') {
		buf++;
		if (!*buf) return 0;
		switch (*buf) {
			case 'b': case 'B': /* byte */
				me->indir.type = byte_type;
				break;
			case 's': case 'S': /* short */
				me->indir.type = short_type;
				break;
			case 'l': case 'L': /* long */
				me->indir.type = long_type;
				break;
			default:
				return NULL;
		}
		
		if ((*buf == 'b') || (*buf == 's') || (*buf == 'l'))
			me->indir.endian = little_endian;
		else me->indir.endian = big_endian;
		
		buf++;
	} else {
		/* default to native endian long */
		me->indir.type = long_type;
		me->indir.endian = native_endian;
	}
		
	/* read delta value, but don't bail if we can't find one */
	if ((b2 = read_number(buf,&me->indir.delta)) == NULL)
		me->indir.delta = 0;
	else
		buf = b2;
	
	/* check for the terminating parenthesis */
	if (*buf != ')') return NULL;
	buf++;

	return buf;
}

/*
 * read_offset
 *
 * Read an absolute, relative or indirect offset.
 *
 * Returns the place in the buffer where reading stopped.
 */
static const char* read_offset (const char* buf, magicentry_t* me)
{
	assert(buf);
	assert(me);
	
	me->indirect=0;
	me->relative=0;

	/* indirect offset */
	if (*buf == '(') {
		buf++;
                if (!(buf = read_indirect_offset(buf,me))) return NULL;
	} else {
		/* relative offset */
		if (*buf == '&') {
			buf++;
			if (me->level > 0) me->relative = 1;
			else return NULL;
		}

		/* read offset */
		if (!(buf = read_number_u(buf,&me->offset))) return NULL;
	}
	
	return buf;
}
	
/*
 * read_type
 *
 * Read the data type and endianness.
 *
 * Returns the place in the buffer where reading stopped.
 */
static const char* read_type (const char* buf, magicentry_t* me)
{

	assert(buf);
	assert(me);

	/* TODO: signed comparisons. */
	if (*buf == 'u') buf++;

	/* find endianness */
	if (strncmp(buf,"be",strlen("be")) == 0) {
		me->num.endian = big_endian;
		buf += strlen("be");
	} else if (strncmp(buf,"le",strlen("le")) == 0) {
		me->num.endian = little_endian;
		buf += strlen("le");
	} else
		me->num.endian = native_endian;

	/* find length */
	if (strncmp(buf,"byte",strlen("byte")) == 0) {
		/* cannot specify endianness with bytes */
		if (me->num.endian != native_endian) return NULL;
		me->type = byte_type;
		buf += strlen("byte");
	} else if (strncmp(buf,"short",strlen("short")) == 0) {
		me->type = short_type;
		buf += strlen("short");
	} else if (strncmp(buf,"long",strlen("long")) == 0) {
		me->type = long_type;
		buf += strlen("long");
	} else if (strncmp(buf,"date",strlen("date")) == 0) {
		me->type = date_type;
		buf += strlen("date");
	} else if (strncmp(buf,"string",strlen("string")) == 0) {
		/* cannot specify endianness with strings */
		if (me->num.endian != native_endian) return NULL;
		me->type = string_type;
		buf += strlen("string");
	} else return NULL; /* unsupported data type */
	
	return buf;
}

/*
 * read_comparison
 *
 * Read the type of comparision to be performed
 *	(equal, less than, greater than, and, xor, not).
 *
 * Returns the place in the buffer where reading finished.
 */
static const char* read_comparison (const char* buf, magicentry_t* me)
{
	assert(buf);
	assert(me);

	if (*buf == '=') {
		buf++;
		me->comp = equ_comp;
	} else if (*buf == '<') {
		buf++;
		me->comp = lt_comp;
	} else if (*buf == '>') {
		buf++;
		me->comp = gt_comp;
	} else if ((*buf == '!') && (me->type != string_type)) {
		buf++;
		me->comp = not_comp;
	} else if (*buf == 'x') {
		buf++;
		me->comp = any_comp;
	} else if ((*buf == '&') && (me->type != string_type)) {
		buf++;
		me->comp = and_comp;
	} else if ((*buf == '^') && (me->type != string_type)) {
		buf++;
		me->comp = xor_comp;
	} else me->comp = equ_comp; /* default to '=' */
	
	return buf;
}

/*
 * read_level
 *
 * Read the test level (number of '>').
 *
 * Returns the place in the buffer where reading finished.
 */
static const char* read_level (const char* buf, magicentry_t* me)
{
	assert(buf);
	assert(me);

	for (me->level = 0; *buf == '>'; me->level++, buf++) ;
	if (!*buf) return NULL;
	
	return buf;
}

/*
 * read_mask
 *
 * Read a mask, but only for integer data types.
 *
 * Returns the place in the buffer where reading finished.
 */
static const char* read_mask (const char* buf, magicentry_t* me)
{
	assert(buf);
	assert(me);

	/* if type is followed by '&', it is masked */
	if (*buf == '&') {
		buf++;
		/* can't mask strings */
		if (me->type == string_type) return NULL;
		/* no mask */
		if (!*buf || isspace(*buf)) return NULL;
		if (!(buf = read_number_u(buf,&me->num.mask))) return NULL;
		
	} else me->num.mask=~0U; /* mask that does nothing */
	
	return buf;
}

/*
 * read_test_value
 *
 * Read either a number or a string, depending on the entry.
 *
 * Returns the place in the buffer where reading finished.
 */
static const char* read_test_value (const char* buf, magicentry_t *me)
{
	assert(buf);
	assert(me);

	/* integer */
	if (me->type != string_type) {
		/* read the number from the file, byteswap it if necessary */
		
		if (!(buf = read_number_u(buf,&me->num.value))) return NULL;
		
#if BYTE_ORDER == BIG_ENDIAN
		if (me->num.endian == little_endian)
#elif BYTE_ORDER == LITTLE_ENDIAN
		if (me->num.endian == big_endian)
#endif
		{
			if ((me->type == long_type) || (me->type == date_type))
				byteswap_4 (&me->num.value);
			else if (me->type == short_type) {
				uint16_t n = (uint16_t)(me->num.value & 0xFFFF);

				byteswap_2 (&n);
				me->num.value = n;
			}
		}
	} else /* string */
		if (!(buf = read_string(buf,&me->str.value,&me->str.length,1,0))) return NULL;	
	
	return buf;
}

/*
 * parse_line
 *
 * Parse a line from a buffer, in the magic(4) format. Returns 0 on success.
 *
 * Returns zero on failure, nonzero on success.
 */
static int parse_line (const char* buf, magicentry_t *me)
{
	assert(buf);
	assert(me);
	
	me->str.value = NULL;
	me->msg = NULL;
	
	/* skip over leading whitespace */
	for (; *buf && isspace(*buf); buf++) ;
	
	if (!*buf) return 0; /* empty line */
	if (*buf == '#') return 0; /* comments */

	if (!(buf = read_level(buf,me))) return 0;
	if (!(buf = read_offset(buf,me))) return 0;
	
	/* should be a space here */
	if ((*buf) && (!isspace(*buf))) return 0;
	
	/* skip over spaces */
	for (; *buf && isspace(*buf); buf++) ;
	if (!*buf) return 0;

	if (!(buf = read_type(buf,me))) return 0;
	if (me->type != string_type)
		if (!(buf = read_mask(buf,me))) return 0;
	
	/* should be a space here */
	if ((*buf) && (!isspace(*buf))) return 0;
	
	/* skip over spaces */
	for (; *buf && isspace(*buf); buf++) ;	
	if (!*buf) return 0;

        if (!(buf = read_comparison(buf,me))) return 0;
	if (me->comp != any_comp) {
        	if (!(buf = read_test_value(buf,me))) return 0;
	} else {
		/* no test value needed, everything passes this test */
		me->num.value = 0U;
		me->str.value = (uint8_t*)strdup("");
		if (!me->str.value) return 0;
		me->str.length = 0;
	}
	
	/* should be a space here */
	if ((*buf) && (!isspace(*buf))) return 0;

	/* skip over spaces */
	for (; *buf && isspace(*buf); buf++) ;
	
	/* check for *optional* description */
	if (!*buf) {
		me->msg = NULL;
		return 1;
	}

	/* read the description (entire remainder of the line) */
	read_string(buf,(uint8_t**)&me->msg,NULL,0,1);

	return 1;
}


/*
 * byteswap_4
 *
 * Byteswap a 4 byte (32 bit) number.
 *
 * Returns the byteswapped number.
 */
static uint32_t byteswap_4 (uint32_t* n)
{
	uint32_t t;
	
	assert(n);

	t = *n;
	*n = (t<<24) | ((t&0xFF00)<<8) | ((t&0xFF0000)>>8) | (t>>24);
	
	return *n;
}

/*
 * byteswap_2
 *
 * Byteswap a 2 byte (16 bit) number.
 *
 * Returns the byteswapped number.
 */
static uint16_t byteswap_2 (uint16_t* n)
{
	uint16_t t;
	
	assert(n);

	t = *n;
	*n = (t>>8) | ((t&0xFF)<<8);

	return *n;
}

/*
 * calculate_offset
 *
 * Calculate the offset at which the data is expected
 *
 * Returns the offset, or -1 if an error occurred.
 */
static int calculate_offset (const magicentry_t* me, unsigned int offset,
	const uint8_t* data, unsigned int len)
{
	unsigned int ofs;
	
	assert(me);
	assert(data);
	
	if (me->relative) {
		ofs = offset + me->offset;		
	} else if (me->indirect) {
		if ((me->indir.type == byte_type) && (me->offset < len)) {
			ofs = data[me->offset];
		} else if ((me->indir.type == short_type) && (me->offset+1 < len)) {
			if (me->indir.endian == native_endian) {
				uint16_t sofs;
				
				sofs = ((uint16_t)data[me->offset] << 8) 
					| ((uint16_t)data[me->offset+1]);
				
#if BYTE_ORDER == LITTLE_ENDIAN
					byteswap_2(&sofs);
#endif
				
				ofs = sofs;
			} else if (me->indir.endian == big_endian) {
				ofs = ((uint16_t)data[me->offset] << 8) 
					| ((uint16_t)data[me->offset+1]);
			} else if (me->indir.endian == little_endian) {
				ofs = (data[me->offset+1] << 8) | (data[me->offset]);
			} else assert(0);
		} else if ((me->indir.type == long_type) && (me->offset+3 < len)) {
			if (me->indir.endian == native_endian) {
				ofs = (data[me->offset] << 24) | (data[me->offset+1] << 16)
					| (data[me->offset+2] << 8) | (data[me->offset+3]);

#if BYTE_ORDER == LITTLE_ENDIAN
					byteswap_4(&ofs);
#endif
			} else if (me->indir.endian == big_endian) {
				ofs = (data[me->offset] << 24) | (data[me->offset+1] << 16)
					| (data[me->offset+2] << 8) | (data[me->offset+3]);
			} else if (me->indir.endian == little_endian) {
				ofs = (data[me->offset+3] << 24) | (data[me->offset+2] << 16)
					| (data[me->offset+1] << 8) | (data[me->offset]);
			} else assert(0);
		} else return -1;
		
		/* negative offsets aren't fun */
		if ((me->indir.delta < 0) && ((unsigned int)-(me->indir.delta) > ofs))
			return -1;
			
		ofs += me->indir.delta;
	} else {
		ofs = me->offset;
	}
	
	return ofs;
}

/*
 * do_string_test
 *
 * Perform a string comparison.
 *
 * Returns the length of the string that matched, or -1 if no match.
 */
static int do_string_test (const magicentry_t* me, unsigned int ofs,
	const uint8_t* data, unsigned int len, char* desc)
{
	int length=0, result=0;
	int r;
	int complen;

	assert(me);
	assert(data);
	assert(desc);

	complen = me->str.length;
	
	/* make sure theres enough room in the buffer for the string */
	if (!(ofs+complen < len))
		return -1;
	
	/* compare what we read from the file to the test value 
	   most of the tests will fail in the character */
	if (((r = (data[ofs]-*me->str.value)) == 0))
		r = memcmp(&data[ofs],me->str.value,complen);

	/* interpret the result according to the comparison operation */
	if (me->comp == equ_comp) result = (r==0);
	else if (me->comp == lt_comp) result = (r<0);
	else if (me->comp == gt_comp) result = (r>0);
	else if (me->comp == any_comp) result = 1;
	else assert(0);

	/* if the comparison succeeded and had a message */
	if ((result) && (me->msg)) {
		/* test for equality */

		if (me->comp == equ_comp) {
			uint8_t* strval;
			
			/* create a C-style (zero terminated) string from
			   me->strval */
			if ((strval = malloc(me->str.length+1)) == NULL)
				return -1;
					
			/* copy and add a terminating nul */
			memcpy(strval,me->str.value,me->str.length);
			*(strval+me->str.length)='\0';
			
			/* account for backspace */
			if ((*desc) && (*me->msg != '\b'))
				strncat(desc," ",MAXDESC-strlen(desc)-1);
				
			/* append to the message */
			snprintf1s(desc+strlen(desc),
				MAXDESC-strlen(desc)-1,
				*me->msg!='\b'?me->msg:me->msg+1,(char*)strval);

			length = strlen((char*)strval);

			free(strval);
		} else { /* not a test for equality */
			int nc=0;
			uint8_t* line;
			
			/* special behaviour if the test value is '\0', next
			   '\n'-terminated line is used in description */
			if (*(me->str.value) == '\0') {			
				uint8_t* end;
				/* terminate it at the next '\n' or if one
				   doesn't exist, terminate at end of buffer */
				end = (uint8_t*)memchr(&data[ofs],'\n',len-ofs);
				if (end) {
					nc = end - &data[ofs];
				} else {
					nc = len-ofs;
				}
			} else {
				uint8_t* end;
				/* terminate it at the next '\0' or if one
				   doesn't exist, terminate at end of buffer */
				end = (uint8_t*)memchr(&data[ofs],'\0',len-ofs);
				if (end) {
					nc = end - &data[ofs];
				} else {
					nc = len-ofs;
				}
			}

			/* allocate space for it, plus one byte for the
			   terminating null */
			if ((line=malloc(nc+1)) == NULL)
				return -1;

			/* copy, null terminate */
			memcpy(line,&data[ofs],nc);
			*(line+nc) = '\0';
			
			/* account for backspace */
			if ((*desc) && (*me->msg != '\b'))
				strncat(desc," ",MAXDESC-strlen(desc)-1);
			
			/* append to description */
			snprintf1s(desc+strlen(desc),MAXDESC-strlen(desc)-1,
				*me->msg!='\b'?me->msg:me->msg+1,(char*)line);

			free(line);
			length=nc;
		}
	}
	
	/* return the length of the string, or -1 if not matched */
	return (result) ? length : -1;
}

/*
 * do_integer_test
 *
 * Perorms an integer comparison.
 *
 * Returns the length of the match or -1 if it failed.
 */
static int do_integer_test (const magicentry_t* me, unsigned int ofs,
	const uint8_t* data, unsigned int len, char* desc)
{
	int length=0, result=0;
	uint32_t n1=0, n2=0, n3 = 0;

	assert(me);
	assert(data);
	assert(desc);
	
	if ((me->type == byte_type) && (ofs<len)) { /* byte */
		/* get byte from buffer */
		n1 = n3 = data[ofs];

		/* apply mask */
		n1&=(uint8_t)(me->num.mask&0xFF);
		
		length=1;
	} else if ((me->type == short_type) && (ofs+1 < len)) { /* short */
		uint16_t n, m, mask = (uint16_t)(me->num.mask&0xFFFF);

		/* get 16 bit big endian short from buffer */
		n = ((uint16_t)data[ofs] << 8) | ((uint16_t)data[ofs+1]);
		
		/* make sure it's in the native byte order */
#if BYTE_ORDER == LITTLE_ENDIAN
		byteswap_2(&n);
		byteswap_2(&mask);
#endif

		/* apply the mask */
		m=n;
		n&=mask;

		/* swap it back if needed */
#if BYTE_ORDER == LITTLE_ENDIAN
		if (me->num.endian == big_endian)
#elif BYTE_ORDER == BIG_ENDIAN
		if (me->num.endian == little_endian)
#endif
			byteswap_2(&m);
		
		n1=n;
		n3=m;
		
		length=2; /* 16/8 */
	} else if (((me->type == long_type) || (me->type == date_type)) &&
		(ofs+3 < len)) { /* long */
		uint32_t mask = me->num.mask;
		
		/* get 32 bit big endian long from buffer */
		n1 = (data[ofs] << 24) | (data[ofs+1] << 16)
			| (data[ofs+2] << 8) | (data[ofs+3]);

		/* make sure it's in native byte order */
#if BYTE_ORDER == LITTLE_ENDIAN
			byteswap_4(&mask);
			byteswap_4(&n1);
#endif
		
		/* apply the mask */
		n3=n1;
		n1&=mask;
		
		/* swap it back if needed */
#if BYTE_ORDER == LITTLE_ENDIAN
		if (me->num.endian == big_endian)
#elif BYTE_ORDER == BIG_ENDIAN
		if (me->num.endian == little_endian)
#endif
			byteswap_4(&n3);
		
		length = 4; /* 32/8 */
	} else return -1;

	/* do the comparison specified */	
	n2 = me->num.value;
	if (me->comp == equ_comp) result = (n1==n2);
	else if (me->comp == lt_comp) result = (n1<n2);
	else if (me->comp == gt_comp) result = (n1>n2);
	else if (me->comp == and_comp) result = (n1&n2)==n2;
	else if (me->comp == xor_comp) result = (n1&n2)!=n2;
	else if (me->comp == any_comp) result = 1;
	else if (me->comp == not_comp) result = (n1!=n2);
	else assert(0);
	
	/* comparison succeeded and had a message */
	if ((result) && (me->msg)) {
		if (me->type != date_type) { /* not date */
			/* account for backspace */
			if ((*desc) && (*me->msg != '\b'))
				strncat(desc," ",MAXDESC-strlen(desc)-1);

			/* append to description */
			snprintf1i(desc+strlen(desc),
				MAXDESC-strlen(desc)-1,
				*me->msg!='\b'?me->msg:me->msg+1,n3);
		} else { /* date */
			/* convert the integer to a date string, remove the
			   annoying '\n' it always sticks on the end */
			char timebuf[64], *eol;

			strncpy(timebuf,ctime((time_t*)&n3),sizeof(timebuf));
			if ((eol = strrchr(timebuf,'\n'))) *eol='\0';
			
			/* account for backspace */
			if ((*desc) && (*me->msg != '\b'))
				strncat(desc," ",MAXDESC-strlen(desc)-1);

			/* append to description */
			snprintf1s(desc+strlen(desc),
				MAXDESC-strlen(desc)-1,
				*me->msg!='\b'?me->msg:me->msg+1,timebuf);
		}
	}	
	
	/* if succeeded, return the length of the match, otherwise -1 */
	return (result) ? length : -1;
}

/*
 * do_test
 *
 * Carry out the specified test on the data, return the offset of the start
 * of the match if successful, otherwise return -1.
 */
static int do_test (const magicentry_t* me, unsigned int prevoffset,
	const uint8_t* data, int len, char* desc)
{
	unsigned int ofs;
	int length=0;
		
	assert(me);
	assert(data);
	assert(desc);

	ofs = calculate_offset(me,prevoffset,data,len);
	
	if (me->type == string_type)
		length = do_string_test (me,ofs,data,len,desc);
        else
		length = do_integer_test (me,ofs,data,len,desc);
	
	return (length>=0) ? (ofs+length) : -1;
}

/*
 * process_test_level_zero
 *
 * Processes a test at level zero, using a sorted list if sorting is turned on.
 *
 * Returns nonzero if done.
 */
static int process_test_level_zero (magichandle_t* mh, unsigned int i,
	int offset, const uint8_t* data, size_t len, char* desc)
{
       	int lastoffset;
       	    	
        assert(mh);
        assert(data);
        assert(desc);

	for (i = 0; i<mh->num_l0_ents; i++) {
		/* if it succeeds, try the subtypes recursively */
		if ((lastoffset=do_test(&mh->mentries[mh->l0_ents[i]],
			offset,data,len,desc)) >= 0) {
			
			if (!(process_test(mh,1,mh->l0_ents[i]+1,
				lastoffset, data, len, desc)))
				
				return 0;
			else { /* last one to try */
				
				if (!(mh->flags & MAGIC_FLAGS_SORT))
					return 1;
					
				/* hit this entry, reorder the list
				   to make the searches faster */		
				if (++mh->mentries[mh->l0_ents[i]].hits == 0xFF)
					rescale_level_zero (mh);
				
				/* already at the top of the list */
				if (i == 0)
					return 1;
					
				/* reorder the list */
				reorder_level_zero (mh, i);
					
				return 1;
			}
		}
	}        	

	return 1;
}

/*
 * process_test_level
 *
 * Process a test at any level.
 *
 * Returns nonzero if done.
 */
static int process_test_level (magichandle_t*mh, unsigned int level, unsigned int i,
	int offset, const uint8_t* data, size_t len, char* desc)
{	
       	int lastoffset;
       	
	assert (mh);
	assert (data);
	assert (desc);
	
	for (; i<mh->numentries; i++) {
		/* make sure the test is on the right level */
		if (level > mh->mentries[i].level) return 1;
		else if (level < mh->mentries[i].level) continue;
		
		/* if the test succeeds, recursively go through the
		   subtypes... */
		if ((lastoffset=do_test(&mh->mentries[i],offset,data,len,
			desc)) >= 0) {
			
			if (!(process_test(mh,level+1, i+1, lastoffset,data,
				len,desc)))
				
				return 0;
		}
	}
	
	return 1;
}

/*
 * process_test
 *
 * Recursively traverse the test `tree'. The first level is done specially
 * to make it faster.
 *
 * Returns nonzero if done.
 */
static int process_test (magichandle_t*mh, unsigned int level, unsigned int i,
	int offset, const uint8_t* data, size_t len, char* desc)
{
	
	assert(mh);
	assert(data);
	assert(desc);
	
	if ((!mh) || (!mh->mentries) || (mh->numentries == 0)) 
		return 1;

	/* no more tests, or no more tests on this level */
	if ((i>mh->numentries) || (mh->mentries[i].level != level))
		return 1;

	/* too many levels of tests  */
	if (level >= MAXLEVEL)
		return 1;
	
	/* special processing for test level zero */
	if (level == 0) return process_test_level_zero (mh, i, offset, data, len, desc);
	else return process_test_level (mh, level, i, offset, data, len, desc);
	
	return 1;
}

/*
 * rescale_level_zero
 *
 * Rescale the hit counts of the level zero list so they don't overflow
 * the integer. All the counts are quickly divided by two with a right shift.
 */
static void rescale_level_zero (magichandle_t* mh)
{
	size_t i;

	assert (mh);
	assert (mh->l0_ents);
	
	for (i=0; i<mh->num_l0_ents; i++)
		mh->mentries[mh->l0_ents[i]].hits>>=1;
}

/*
 * reorder_level_zero
 *
 * Get the level zero list back into descending order quickly.
 */
static void reorder_level_zero (magichandle_t* mh, unsigned int changed)
{
	unsigned int j;
	
	for (j = changed - 1;; j--) {
		/* the changed entry is bigger than this, swap them */
		if (mh->l0_ents[changed] >= mh->l0_ents[j]) {
			unsigned tmp;
			
			tmp = mh->l0_ents[changed];
			mh->l0_ents[changed] = mh->l0_ents[j];
			mh->l0_ents[j] = tmp;
		} else break;
		
		if (j == 0) break;
	}
}
