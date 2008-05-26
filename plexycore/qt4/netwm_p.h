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

  Copyright (c) 2000 Troll Tech AS
  Copyright (c) 2003 Lubos Lunak <l.lunak@kde.org>

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

*/


#ifndef   __net_wm_p_h
#define   __net_wm_p_h

/**
   Resizable array class.

   This resizable array is used to simplify the implementation.  The existence of
   this class is to keep the implementation from depending on a separate
   framework/library.
   @internal
**/

template <class Z> class NETRArray
{
public:
        /**
           Constructs an empty (size == 0) array.
        **/

        NETRArray();

        /**
           Resizable array destructor.
        **/

        ~NETRArray();

        /**
           The [] operator does the work.  If the index is larger than the current
           size of the array, it is resized.
         **/

        Z &operator[](int);

        /**
           Returns the size of the array.
         **/

        int size() const {
                return sz;
        }

        /**
           Resets the array (size == 0).
         **/
        void reset();

private:
        int sz;
        int capacity;
        Z *d;
};


/**
   Private data for the NETRootInfo class.
   @internal
**/

struct NETRootInfoPrivate {
        // information about the X server
        Display *display;
        NETSize rootSize;
        Window root;
        Window supportwindow;
        const char *name;
        int screen;

        // data that changes (either by the window manager or by a client)
        // and requires updates
        NETRArray<NETPoint> viewport;
        NETRArray<NETRect> workarea;
        NETSize geometry;
        Window active;
        Window *clients, *stacking, *virtual_roots, *kde_system_tray_windows;
        NETRArray<const char *> desktop_names;
        int number_of_desktops;
        int current_desktop;

        unsigned long clients_count, stacking_count, virtual_roots_count,
        kde_system_tray_windows_count;
        bool showing_desktop;

        unsigned long properties[ 5 ];
        unsigned long client_properties[ 5 ]; // properties the client is interested in

        int ref;
};


/**
   Private data for the NETWinInfo class.
   @internal
**/

struct NETWinInfoPrivate {
        Display *display;
        Window window, root;
        NET::MappingState mapping_state;
        Bool mapping_state_dirty;

        NETRArray<NETIcon> icons;
        int icon_count;

        NETRect icon_geom, win_geom;
        unsigned long state;
        NETExtendedStrut extended_strut;
        NETStrut strut;
        NETStrut frame_strut; // strut?
        NETRArray<NET::WindowType> types;
        char *name, *visible_name, *icon_name, *visible_icon_name;
        int desktop;
        int pid;
        int handled_icons;
        Window kde_system_tray_win_for;
        Time user_time;
        char* startup_id;
        Window transient_for, window_group;
        unsigned long allowed_actions;
        char* class_class, *class_name, *role, *client_machine;

        unsigned long properties[ 2 ];
        bool has_net_support;

        int ref;
};


#endif // __net_wm_p_h
