/*
 *  minifile - Minimal file(1) command using libmagic 
 *  $Id: minifile.c,v 1.1 2000/05/26 07:30:53 fyre Exp $
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
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "magic.h"

int main (int argc, char *argv[]) {
	magichandle_t* mh;
	const char* d;
	int i;
	
	if (argc < 2) {
		fprintf (stderr, "magic: no files specified\n");
		fprintf (stderr, "usage: minifile files...\n");
		return 1;
	}

	mh = magic_init (MAGIC_FLAGS_NONE);
	magic_read_entries(mh,"magic");

	for (i = 1; i < argc; i++) {
		d = magic_identify_file (mh, argv[i]);
		printf("%s: %s\n",argv[i],d ? d : "unknown");
	}

	magic_close (mh);
	
	return 0;
}

