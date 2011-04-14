/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
/*
 * PlexyDesk
 *
 * Sci-Fi Desktop for the Hurd
 *
 * Authored By Siraj Razick <siraj@kde.org>
 *      Dariusz Mikulski <dariusz.mikulski@gmail.com>
 *
 * Copyright (C) 2006 PlexyDeskTeam
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

#include <config.h>
#include <QtCore/QtGlobal>

#ifdef Q_WS_WIN
#define __PRETTY_FUNCTION__ QString("%1 %2 %3 %4 %4").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__).arg(__FUNCDNAME__).arg(__FUNCSIG__)
#endif

#ifdef __GNUC__
#define PACK __attribute__((__packed__))
#endif

#define __SYSTEM_HAVE_GCC_VISIBILITY

#if defined(_WIN32)
  #if defined(plexydeskcore_EXPORTS)
    #define  PLEXYDESK_EXPORT __declspec(dllexport)
  #else
    #define  PLEXYDESK_EXPORT __declspec(dllimport)
  #endif
#else
    #ifdef __SYSTEM_HAVE_GCC_VISIBILITY
        #define HIDDEN_SYM __attribute__ ((visibility("hidden")))
        #define VISIBLE_SYM __attribute__ ((visibility("default")))
        #define PLEXYDESK_EXPORT __attribute__ ((visibility("default")))
    #endif
#endif

//TODO
//add dllexport for windows
/*
 * this  provides a helper macro for exporting plugin Classes
 * example usage  PLUGIN_EXPORT(libmyone,ClassName)
 * you will find many example usages as we include them in this
 * source distrubution.
 * */

#define PLUGIN_EXPORT(name, sym) \
    extern "C" { void *setup_ ## name(){return new sym; }}

#define  Q_UINT32 unsigned int

#ifdef Q_WS_WIN
#include <QCoreApplication>

static QString windowsPath = QString("%1%2").arg(PLEXPREFIX).arg(QString("/.."));
#undef PLEXPREFIX

#define PLEXPREFIX windowsPath

#endif

namespace Plexy
{

}

#endif
