/*
 *  libmagic - automagically determine file type
 *  $Id: magic.h,v 1.18 2000/05/26 07:34:09 fyre Exp $
 *
 *  Copyright (c) 2000 Tim Robbins 		<tim_r@hotmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
 
#ifndef INCLUDED_MAGIC_H
#define INCLUDED_MAGIC_H

#include "sysdep.h"

/* limits that only we need to know about */
#ifdef MAGIC_C
#define HOWMANY			0x2000
#define MAXLINE			0x200
#define MAXSTRING		0x100
#define MAXDESC			0x100
#define MAXLEVEL		0x10
#define MAXENTRIES		0x4000
#define GROWBY			0x40	
#endif

/* flags, passed to magic_init() */
#define MAGIC_FLAGS_NONE	0x0
#define MAGIC_FLAGS_SORT	0x1

typedef enum {
	native_endian, big_endian, little_endian
} endian_t;

typedef enum {
	byte_type, short_type, long_type, date_type, string_type
} type_t;

typedef enum {
	equ_comp, lt_comp, gt_comp, and_comp, xor_comp, any_comp, not_comp
} comp_t;

typedef struct {
	/* test level */
	unsigned level;

	/* number of times this entry has been successfully matched,
	   only used where level == 0 */
	uint8_t hits;
	
	/* offset: interpreted differently depending on `indirect' and
	   `relative' */
	unsigned offset;

	type_t type;	
	comp_t comp;
	
	/* numeric values: byte, short, long, date */
	struct {
		endian_t endian;
		unsigned int value;
		uint32_t mask;
	} num;
	
	/* string values */
	struct {
		uint8_t* value;
		size_t length;
	} str;
	
	char* msg;	
	
	unsigned indirect:1;
	struct {
		endian_t endian;
		type_t type;
		int delta;
	} indir;
	
	unsigned relative:1;

} magicentry_t;

typedef struct {
	/* flags */
	unsigned flags;
	
	/* number of magic entries */
	size_t numentries;
	
	/* number of level 0 entries */
	size_t num_l0_ents;
	/* list of level 0 entries sorted by `hits' */
	size_t* l0_ents;
	
	/* the entries */
	magicentry_t* mentries;
	
	/* a buffer to hold the description */
	char desc[512];
} magichandle_t;

magichandle_t* magic_init (unsigned flags);
int magic_close (magichandle_t* mh);
int magic_read_entries (magichandle_t* mh, const char* fn);
const char* magic_identify_file (magichandle_t* mh, const char* fn);
const char* magic_identify_buffer (magichandle_t* mh, const uint8_t* buf, size_t buflen);
int magic_add_entry_string (magichandle_t* mh, const char* ent);

#endif
