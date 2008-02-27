/*
 *  libmagic - system dependent things 
 *  $Id: sysdep.h,v 1.5 2000/05/26 07:34:55 fyre Exp $
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
 
#ifndef INCLUDED_SYSDEP_H
#define INCLUDED_SYSDEP_H

#define _GNU_SOURCE

/* standard integer sizes */
#if (__GLIBC__ >= 2)
# include <stdint.h>
#else
/* define them ourselves */
/* CHANGEME */
typedef signed char    		int8_t;
typedef short int     		int16_t;
typedef int           		int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int     uint16_t;
typedef unsigned int           uint32_t;
#endif

#if !defined(BYTE_ORDER) || !defined(LITTLE_ENDIAN) || !defined(BIG_ENDIAN)
# if (__GLIBC__ >= 2) 
#  include <endian.h>
# else
#  if (BSD >= 199103) || defined(__FreeBSD__) || defined(__OpenBSD__)
#   include <machine/endian.h>
#  else
#   define BIG_ENDIAN		4321
#   define LITTLE_ENDIAN	1234
#   error Define your byte order in sysdep.h 
/*
#   define BYTE_ORDER	LITTLE_ENDIAN
#   define BYTE_ORDER	BIG_ENDIAN
*/
#  endif
# endif
#endif

#endif
