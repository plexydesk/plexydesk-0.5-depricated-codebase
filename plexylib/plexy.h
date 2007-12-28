/*
 * PlexyDesk
 *
 * Sci-Fi Desktop for the Hurd
 *
 * Authored By Siraj Razick <siraj@kde.org>
 *
 * Copyright (C) 2006 OpenedHand
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* visibility options
 * note only support GCC 4.x and not Windows 
 * */

#ifndef __PLEXY_EXPORTS
#define __PLEXY_EXPORTS

#ifdef __GNUC_
#define PACK __attribute__((__packed__))
#endif 

#define __SYSTEM_HAVE_GCC_VISIBILITY

#ifdef __SYSTEM_HAVE_GCC_VISIBILITY
#define HIDDEN_SYM __attribute__ ((visibility("hidden")))
#define VISIBLE_SYM __attribute__ ((visibility("default")))
#endif

//TODO
//add dllexport for windows


namespace Plexy
{



}


#endif
