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

//#define NETWMDEBUG

#include <qwidget.h>
#ifdef Q_WS_X11 //FIXME

#include "netwm.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <X11/Xmd.h>

#include "netwm_p.h"

// UTF-8 string
static Atom UTF8_STRING = 0;

// root window properties
static Atom net_supported            = 0;
static Atom net_client_list          = 0;
static Atom net_client_list_stacking = 0;
static Atom net_desktop_geometry     = 0;
static Atom net_desktop_viewport     = 0;
static Atom net_current_desktop      = 0;
static Atom net_desktop_names        = 0;
static Atom net_number_of_desktops   = 0;
static Atom net_active_window        = 0;
static Atom net_workarea             = 0;
static Atom net_supporting_wm_check  = 0;
static Atom net_virtual_roots        = 0;
static Atom net_showing_desktop      = 0;

// root window messages
static Atom net_close_window         = 0;
static Atom net_restack_window       = 0;
static Atom net_wm_moveresize        = 0;
static Atom net_moveresize_window    = 0;

// application window properties
static Atom net_wm_name              = 0;
static Atom net_wm_visible_name      = 0;
static Atom net_wm_icon_name         = 0;
static Atom net_wm_visible_icon_name = 0;
static Atom net_wm_desktop           = 0;
static Atom net_wm_window_type       = 0;
static Atom net_wm_state             = 0;
static Atom net_wm_strut             = 0;
static Atom net_wm_extended_strut    = 0; // the atom is called _NET_WM_STRUT_PARTIAL
static Atom net_wm_icon_geometry     = 0;
static Atom net_wm_icon              = 0;
static Atom net_wm_pid               = 0;
static Atom net_wm_user_time         = 0;
static Atom net_wm_handled_icons     = 0;
static Atom net_startup_id           = 0;
static Atom net_wm_allowed_actions   = 0;
static Atom wm_window_role           = 0;
static Atom net_frame_extents        = 0;

// KDE extensions
static Atom kde_net_system_tray_windows       = 0;
static Atom kde_net_wm_system_tray_window_for = 0;
static Atom kde_net_wm_frame_strut            = 0;
static Atom kde_net_wm_window_type_override   = 0;
static Atom kde_net_wm_window_type_topmenu    = 0;
static Atom kde_net_wm_temporary_rules        = 0;

// application protocols
static Atom wm_protocols = 0;
static Atom net_wm_ping = 0;
static Atom net_wm_take_activity = 0;

// application window types
static Atom net_wm_window_type_normal  = 0;
static Atom net_wm_window_type_desktop = 0;
static Atom net_wm_window_type_dock    = 0;
static Atom net_wm_window_type_toolbar = 0;
static Atom net_wm_window_type_menu    = 0;
static Atom net_wm_window_type_dialog  = 0;
static Atom net_wm_window_type_utility = 0;
static Atom net_wm_window_type_splash  = 0;

// application window state
static Atom net_wm_state_modal        = 0;
static Atom net_wm_state_sticky       = 0;
static Atom net_wm_state_max_vert     = 0;
static Atom net_wm_state_max_horiz    = 0;
static Atom net_wm_state_shaded       = 0;
static Atom net_wm_state_skip_taskbar = 0;
static Atom net_wm_state_skip_pager   = 0;
static Atom net_wm_state_hidden       = 0;
static Atom net_wm_state_fullscreen   = 0;
static Atom net_wm_state_above        = 0;
static Atom net_wm_state_below        = 0;
static Atom net_wm_state_demands_attention = 0;

// allowed actions
static Atom net_wm_action_move        = 0;
static Atom net_wm_action_resize      = 0;
static Atom net_wm_action_minimize    = 0;
static Atom net_wm_action_shade       = 0;
static Atom net_wm_action_stick       = 0;
static Atom net_wm_action_max_vert    = 0;
static Atom net_wm_action_max_horiz   = 0;
static Atom net_wm_action_fullscreen  = 0;
static Atom net_wm_action_change_desk = 0;
static Atom net_wm_action_close       = 0;

// KDE extension that's not in the specs - Replaced by state_above now?
static Atom net_wm_state_stays_on_top = 0;

// used to determine whether application window is managed or not
static Atom xa_wm_state = 0;

static Bool netwm_atoms_created      = False;
const unsigned long netwm_sendevent_mask = (SubstructureRedirectMask |
        SubstructureNotifyMask);


const long MAX_PROP_SIZE = 100000;

static char *nstrdup(const char *s1)
{
    if (! s1) return (char *) 0;
    int l = strlen(s1) + 1;
    char *s2 = new char[l];
    strncpy(s2, s1, l);
    return s2;
}


static char *nstrndup(const char *s1, int l)
{
    if (! s1 || l == 0) return (char *) 0;
    char *s2 = new char[l+1];
    strncpy(s2, s1, l);
    s2[l] = '\0';
    return s2;
}


static Window *nwindup(Window *w1, int n)
{
    if (! w1 || n == 0) return (Window *) 0;
    Window *w2 = new Window[n];
    while (n--) w2[n] = w1[n];
    return w2;
}


static void refdec_nri(NETRootInfoPrivate *p)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NET: decrementing NETRootInfoPrivate::ref (%d)\n", p->ref - 1);
#endif
    if (! --p->ref) {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NET: \tno more references, deleting\n");
#endif
        delete [] p->name;
        delete [] p->stacking;
        delete [] p->clients;
        delete [] p->virtual_roots;
        delete [] p->kde_system_tray_windows;
        int i;
        for (i = 0; i < p->desktop_names.size(); i++)
            delete [] p->desktop_names[i];
    }
}


static void refdec_nwi(NETWinInfoPrivate *p)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NET: decrementing NETWinInfoPrivate::ref (%d)\n", p->ref - 1);
#endif
    if (! --p->ref) {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NET: \tno more references, deleting\n");
#endif
        delete [] p->name;
        delete [] p->visible_name;
        delete [] p->icon_name;
        delete [] p->visible_icon_name;
        delete [] p->startup_id;
        int i;
        for (i = 0; i < p->icons.size(); i++)
            delete [] p->icons[i].data;
    }
}


static int wcmp(const void *a, const void *b)
{
    return *((Window *) a) - *((Window *) b);
}


static const int netAtomCount = 77;
static void create_atoms(Display *d)
{
    static const char * const names[netAtomCount] = {
        "UTF8_STRING",
        "_NET_SUPPORTED",
        "_NET_SUPPORTING_WM_CHECK",
        "_NET_CLIENT_LIST",
        "_NET_CLIENT_LIST_STACKING",
        "_NET_NUMBER_OF_DESKTOPS",
        "_NET_DESKTOP_GEOMETRY",
        "_NET_DESKTOP_VIEWPORT",
        "_NET_CURRENT_DESKTOP",
        "_NET_DESKTOP_NAMES",
        "_NET_ACTIVE_WINDOW",
        "_NET_WORKAREA",
        "_NET_VIRTUAL_ROOTS",
        "_NET_SHOWING_DESKTOP",
        "_NET_CLOSE_WINDOW",
        "_NET_RESTACK_WINDOW",

        "_NET_WM_MOVERESIZE",
        "_NET_MOVERESIZE_WINDOW",
        "_NET_WM_NAME",
        "_NET_WM_VISIBLE_NAME",
        "_NET_WM_ICON_NAME",
        "_NET_WM_VISIBLE_ICON_NAME",
        "_NET_WM_DESKTOP",
        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_STATE",
        "_NET_WM_STRUT",
        "_NET_WM_STRUT_PARTIAL",
        "_NET_WM_ICON_GEOMETRY",
        "_NET_WM_ICON",
        "_NET_WM_PID",
        "_NET_WM_USER_TIME",
        "_NET_WM_HANDLED_ICONS",
        "_NET_STARTUP_ID",
        "_NET_WM_ALLOWED_ACTIONS",
        "_NET_WM_PING",
        "_NET_WM_TAKE_ACTIVITY",
        "WM_WINDOW_ROLE",
        "_NET_FRAME_EXTENTS",

        "_NET_WM_WINDOW_TYPE_NORMAL",
        "_NET_WM_WINDOW_TYPE_DESKTOP",
        "_NET_WM_WINDOW_TYPE_DOCK",
        "_NET_WM_WINDOW_TYPE_TOOLBAR",
        "_NET_WM_WINDOW_TYPE_MENU",
        "_NET_WM_WINDOW_TYPE_DIALOG",
        "_NET_WM_WINDOW_TYPE_UTILITY",
        "_NET_WM_WINDOW_TYPE_SPLASH",

        "_NET_WM_STATE_MODAL",
        "_NET_WM_STATE_STICKY",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_SHADED",
        "_NET_WM_STATE_SKIP_TASKBAR",
        "_NET_WM_STATE_SKIP_PAGER",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_FULLSCREEN",
        "_NET_WM_STATE_ABOVE",
        "_NET_WM_STATE_BELOW",
        "_NET_WM_STATE_DEMANDS_ATTENTION",

        "_NET_WM_ACTION_MOVE",
        "_NET_WM_ACTION_RESIZE",
        "_NET_WM_ACTION_MINIMIZE",
        "_NET_WM_ACTION_SHADE",
        "_NET_WM_ACTION_STICK",
        "_NET_WM_ACTION_MAXIMIZE_VERT",
        "_NET_WM_ACTION_MAXIMIZE_HORZ",
        "_NET_WM_ACTION_FULLSCREEN",
        "_NET_WM_ACTION_CHANGE_DESKTOP",
        "_NET_WM_ACTION_CLOSE",

        "_NET_WM_STATE_STAYS_ON_TOP",

        "_KDE_NET_SYSTEM_TRAY_WINDOWS",
        "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR",
        "_KDE_NET_WM_FRAME_STRUT",
        "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE",
        "_KDE_NET_WM_WINDOW_TYPE_TOPMENU",
        "_KDE_NET_WM_TEMPORARY_RULES",

        "WM_STATE",
        "WM_PROTOCOLS"
    };
    Atom atoms[netAtomCount], *atomsp[netAtomCount] = {
        &UTF8_STRING,
        &net_supported,
        &net_supporting_wm_check,
        &net_client_list,
        &net_client_list_stacking,
        &net_number_of_desktops,
        &net_desktop_geometry,
        &net_desktop_viewport,
        &net_current_desktop,
        &net_desktop_names,
        &net_active_window,
        &net_workarea,
        &net_virtual_roots,
        &net_showing_desktop,
        &net_close_window,
        &net_restack_window,

        &net_wm_moveresize,
        &net_moveresize_window,
        &net_wm_name,
        &net_wm_visible_name,
        &net_wm_icon_name,
        &net_wm_visible_icon_name,
        &net_wm_desktop,
        &net_wm_window_type,
        &net_wm_state,
        &net_wm_strut,
        &net_wm_extended_strut,
        &net_wm_icon_geometry,
        &net_wm_icon,
        &net_wm_pid,
        &net_wm_user_time,
        &net_wm_handled_icons,
        &net_startup_id,
        &net_wm_allowed_actions,
        &net_wm_ping,
        &net_wm_take_activity,
        &wm_window_role,
        &net_frame_extents,

        &net_wm_window_type_normal,
        &net_wm_window_type_desktop,
        &net_wm_window_type_dock,
        &net_wm_window_type_toolbar,
        &net_wm_window_type_menu,
        &net_wm_window_type_dialog,
        &net_wm_window_type_utility,
        &net_wm_window_type_splash,

        &net_wm_state_modal,
        &net_wm_state_sticky,
        &net_wm_state_max_vert,
        &net_wm_state_max_horiz,
        &net_wm_state_shaded,
        &net_wm_state_skip_taskbar,
        &net_wm_state_skip_pager,
        &net_wm_state_hidden,
        &net_wm_state_fullscreen,
        &net_wm_state_above,
        &net_wm_state_below,
        &net_wm_state_demands_attention,

        &net_wm_action_move,
        &net_wm_action_resize,
        &net_wm_action_minimize,
        &net_wm_action_shade,
        &net_wm_action_stick,
        &net_wm_action_max_vert,
        &net_wm_action_max_horiz,
        &net_wm_action_fullscreen,
        &net_wm_action_change_desk,
        &net_wm_action_close,

        &net_wm_state_stays_on_top,

        &kde_net_system_tray_windows,
        &kde_net_wm_system_tray_window_for,
        &kde_net_wm_frame_strut,
        &kde_net_wm_window_type_override,
        &kde_net_wm_window_type_topmenu,
        &kde_net_wm_temporary_rules,

        &xa_wm_state,
        &wm_protocols
    };
    assert(!netwm_atoms_created);
    int i = netAtomCount;
    while (i--)
        atoms[i] = 0;
    XInternAtoms(d, (char **) names, netAtomCount, False, atoms);
    i = netAtomCount;
    while (i--)
        *atomsp[i] = atoms[i];
    netwm_atoms_created = True;
}


static void readIcon(Display* display, Window window, Atom property, NETRArray<NETIcon>& icons, int& icon_count)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NET: readIcon\n");
#endif
    Atom type_ret;
    int format_ret;
    unsigned long nitems_ret = 0, after_ret = 0;
    unsigned char *data_ret = 0;
    // reset
    for (int i = 0; i < icons.size(); i++)
        delete [] icons[i].data;
    icons.reset();
    icon_count = 0;
    // allocate buffers
    unsigned char *buffer = 0;
    unsigned long offset = 0;
    unsigned long buffer_offset = 0;
    unsigned long bufsize = 0;
    // read data
    do {
        if (XGetWindowProperty(display, window, property, offset,
                               MAX_PROP_SIZE, False, XA_CARDINAL, &type_ret,
                               &format_ret, &nitems_ret, &after_ret, &data_ret)
                == Success) {
            if (!bufsize) {
                if (nitems_ret < 3 || type_ret != XA_CARDINAL ||
                        format_ret != 32) {
                    // either we didn't get the property, or the property has less than
                    // 3 elements in it
                    // NOTE: 3 is the ABSOLUTE minimum:
                    //     width = 1, height = 1, length(data) = 1 (width * height)
                    if (data_ret)
                        XFree(data_ret);
                    return;
                }
                bufsize = nitems_ret * sizeof(long) + after_ret;
                buffer = (unsigned char *) malloc(bufsize);
            } else if (buffer_offset + nitems_ret*sizeof(long) > bufsize) {
                fprintf(stderr, "NETWM: Warning readIcon() needs buffer adjustment!\n");
                bufsize = buffer_offset + nitems_ret * sizeof(long) + after_ret;
                buffer = (unsigned char *) realloc(buffer, bufsize);
            }
            memcpy((buffer + buffer_offset), data_ret, nitems_ret * sizeof(long));
            buffer_offset += nitems_ret * sizeof(long);
            offset += nitems_ret;
            if (data_ret)
                XFree(data_ret);
        } else {
            if (buffer)
                free(buffer);
            return; // Some error occurred cq. property didn't exist.
        }
    } while (after_ret > 0);
    CARD32 *data32;
    unsigned long i, j, k, sz, s;
    unsigned long *d = (unsigned long *) buffer;
    for (i = 0, j = 0; i < bufsize; i++) {
        icons[j].size.width = *d++;
        i += sizeof(long);
        icons[j].size.height = *d++;
        i += sizeof(long);
        sz = icons[j].size.width * icons[j].size.height;
        s = sz * sizeof(long);
        if (i + s - 1 > bufsize || sz == 0 || sz > 1024 * 1024) {
            break;
        }
        delete [] icons[j].data;
        data32 = new CARD32[sz];
        icons[j].data = (unsigned char *) data32;
        for (k = 0; k < sz; k++, i += sizeof(long)) {
            *data32++ = (CARD32) * d++;
        }
        j++;
        icon_count++;
    }
#ifdef    NETWMDEBUG
    fprintf(stderr, "NET: readIcon got %d icons\n", icon_count);
#endif
    free(buffer);
}


template <class Z>
NETRArray<Z>::NETRArray()
        : sz(0),  capacity(2)
{
    d = (Z*) calloc(capacity, sizeof(Z)); // allocate 2 elts and set to zero
}


template <class Z>
NETRArray<Z>::~NETRArray()
{
    free(d);
}


template <class Z>
void NETRArray<Z>::reset()
{
    sz = 0;
    capacity = 2;
    d = (Z*) realloc(d, sizeof(Z) * capacity);
    memset((void*) d, 0, sizeof(Z)*capacity);
}

template <class Z>
Z &NETRArray<Z>::operator[](int index)
{
    if (index >= capacity) {
        // allocate space for the new data
        // open table has amortized O(1) access time
        // when N elements appended consecutively -- exa
        int newcapacity = 2 * capacity > index + 1 ? 2 * capacity : index + 1; // max
        // copy into new larger memory block using realloc
        d = (Z*) realloc(d, sizeof(Z) * newcapacity);
        memset((void*) &d[capacity], 0, sizeof(Z)*(newcapacity - capacity));
        capacity = newcapacity;
    }
    if (index >= sz)            // at this point capacity>index
        sz = index + 1;
    return d[index];
}


// Construct a new NETRootInfo object.

NETRootInfo::NETRootInfo(Display *display, Window supportWindow, const char *wmName,
                         const unsigned long properties[], int properties_size,
                         int screen, bool doActivate)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::NETRootInfo: using window manager constructor\n");
#endif
    p = new NETRootInfoPrivate;
    p->ref = 1;
    p->display = display;
    p->name = nstrdup(wmName);
    if (screen != -1) {
        p->screen = screen;
    } else {
        p->screen = DefaultScreen(p->display);
    }
    p->root = RootWindow(p->display, p->screen);
    p->supportwindow = supportWindow;
    p->number_of_desktops = p->current_desktop = 0;
    p->active = None;
    p->clients = p->stacking = p->virtual_roots = (Window *) 0;
    p->clients_count = p->stacking_count = p->virtual_roots_count = 0;
    p->kde_system_tray_windows = 0;
    p->kde_system_tray_windows_count = 0;
    p->showing_desktop = false;
    setDefaultProperties();
    if (properties_size > PROPERTIES_SIZE) {
        fprintf(stderr, "NETRootInfo::NETRootInfo(): properties array too large\n");
        properties_size = PROPERTIES_SIZE;
    }
    for (int i = 0; i < properties_size; ++i)
        p->properties[ i ] = properties[ i ];
    // force support for Supported and SupportingWMCheck for window managers
    p->properties[ PROTOCOLS ] |= (Supported | SupportingWMCheck);
    p->client_properties[ PROTOCOLS ] = DesktopNames // the only thing that can be changed by clients
                                        | WMPing; // or they can reply to this
    p->client_properties[ PROTOCOLS2 ] = WM2TakeActivity;
    role = WindowManager;
    if (! netwm_atoms_created) create_atoms(p->display);
    if (doActivate) activate();
}

NETRootInfo::NETRootInfo(Display *display, Window supportWindow, const char *wmName,
                         unsigned long properties, int screen, bool doActivate)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::NETRootInfo: using window manager constructor\n");
#endif
    p = new NETRootInfoPrivate;
    p->ref = 1;
    p->display = display;
    p->name = nstrdup(wmName);
    if (screen != -1) {
        p->screen = screen;
    } else {
        p->screen = DefaultScreen(p->display);
    }
    p->root = RootWindow(p->display, p->screen);
    p->supportwindow = supportWindow;
    p->number_of_desktops = p->current_desktop = 0;
    p->active = None;
    p->clients = p->stacking = p->virtual_roots = (Window *) 0;
    p->clients_count = p->stacking_count = p->virtual_roots_count = 0;
    p->kde_system_tray_windows = 0;
    p->kde_system_tray_windows_count = 0;
    p->showing_desktop = false;
    setDefaultProperties();
    p->properties[ PROTOCOLS ] = properties;
    // force support for Supported and SupportingWMCheck for window managers
    p->properties[ PROTOCOLS ] |= (Supported | SupportingWMCheck);
    p->client_properties[ PROTOCOLS ] = DesktopNames // the only thing that can be changed by clients
                                        | WMPing; // or they can reply to this
    p->client_properties[ PROTOCOLS2 ] = WM2TakeActivity;
    role = WindowManager;
    if (! netwm_atoms_created) create_atoms(p->display);
    if (doActivate) activate();
}


NETRootInfo::NETRootInfo(Display *display, const unsigned long properties[], int properties_size,
                         int screen, bool doActivate)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::NETRootInfo: using Client constructor\n");
#endif
    p = new NETRootInfoPrivate;
    p->ref = 1;
    p->name = 0;
    p->display = display;
    if (screen != -1) {
        p->screen = screen;
    } else {
        p->screen = DefaultScreen(p->display);
    }
    p->root = RootWindow(p->display, p->screen);
    p->rootSize.width = WidthOfScreen(ScreenOfDisplay(p->display, p->screen));
    p->rootSize.height = HeightOfScreen(ScreenOfDisplay(p->display, p->screen));
    p->supportwindow = None;
    p->number_of_desktops = p->current_desktop = 0;
    p->active = None;
    p->clients = p->stacking = p->virtual_roots = (Window *) 0;
    p->clients_count = p->stacking_count = p->virtual_roots_count = 0;
    p->kde_system_tray_windows = 0;
    p->kde_system_tray_windows_count = 0;
    p->showing_desktop = false;
    setDefaultProperties();
    if (properties_size > 2) {
        fprintf(stderr, "NETWinInfo::NETWinInfo(): properties array too large\n");
        properties_size = 2;
    }
    for (int i = 0; i < properties_size; ++i)
        // remap from [0]=NET::Property,[1]=NET::Property2
        switch (i) {
        case 0:
            p->client_properties[ PROTOCOLS ] = properties[ i ];
            break;
        case 1:
            p->client_properties[ PROTOCOLS2 ] = properties[ i ];
            break;
        }
    for (int i = 0; i < PROPERTIES_SIZE; ++i)
        p->properties[ i ] = 0;
    role = Client;
    if (! netwm_atoms_created) create_atoms(p->display);
    if (doActivate) activate();
}

NETRootInfo::NETRootInfo(Display *display, unsigned long properties, int screen,
                         bool doActivate)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::NETRootInfo: using Client constructor\n");
#endif
    p = new NETRootInfoPrivate;
    p->ref = 1;
    p->name = 0;
    p->display = display;
    if (screen != -1) {
        p->screen = screen;
    } else {
        p->screen = DefaultScreen(p->display);
    }
    p->root = RootWindow(p->display, p->screen);
    p->rootSize.width = WidthOfScreen(ScreenOfDisplay(p->display, p->screen));
    p->rootSize.height = HeightOfScreen(ScreenOfDisplay(p->display, p->screen));
    p->supportwindow = None;
    p->number_of_desktops = p->current_desktop = 0;
    p->active = None;
    p->clients = p->stacking = p->virtual_roots = (Window *) 0;
    p->clients_count = p->stacking_count = p->virtual_roots_count = 0;
    p->kde_system_tray_windows = 0;
    p->kde_system_tray_windows_count = 0;
    p->showing_desktop = false;
    setDefaultProperties();
    p->client_properties[ PROTOCOLS ] = properties;
    for (int i = 0; i < PROPERTIES_SIZE; ++i)
        p->properties[ i ] = 0;
    role = Client;
    if (! netwm_atoms_created) create_atoms(p->display);
    if (doActivate) activate();
}


NETRootInfo2::NETRootInfo2(Display *display, Window supportWindow, const char *wmName,
                           unsigned long properties[], int properties_size,
                           int screen, bool doActivate)
        : NETRootInfo(display, supportWindow, wmName, properties, properties_size,
                      screen, doActivate)
{
}

NETRootInfo2::NETRootInfo2(Display *display, const unsigned long properties[], int properties_size,
                           int screen, bool doActivate)
        : NETRootInfo(display, properties, properties_size, screen, doActivate)
{
}

NETRootInfo3::NETRootInfo3(Display *display, Window supportWindow, const char *wmName,
                           unsigned long properties[], int properties_size,
                           int screen, bool doActivate)
        : NETRootInfo2(display, supportWindow, wmName, properties, properties_size,
                       screen, doActivate)
{
}

NETRootInfo3::NETRootInfo3(Display *display, const unsigned long properties[], int properties_size,
                           int screen, bool doActivate)
        : NETRootInfo2(display, properties, properties_size, screen, doActivate)
{
}

NETRootInfo4::NETRootInfo4(Display *display, Window supportWindow, const char *wmName,
                           unsigned long properties[], int properties_size,
                           int screen, bool doActivate)
        : NETRootInfo3(display, supportWindow, wmName, properties, properties_size,
                       screen, doActivate)
{
}

NETRootInfo4::NETRootInfo4(Display *display, const unsigned long properties[], int properties_size,
                           int screen, bool doActivate)
        : NETRootInfo3(display, properties, properties_size, screen, doActivate)
{
}

// Copy an existing NETRootInfo object.

NETRootInfo::NETRootInfo(const NETRootInfo &rootinfo)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::NETRootInfo: using copy constructor\n");
#endif
    p = rootinfo.p;
    role = rootinfo.role;
    p->ref++;
}


// Be gone with our NETRootInfo.

NETRootInfo::~NETRootInfo()
{
    refdec_nri(p);
    if (! p->ref) delete p;
}


void NETRootInfo::setDefaultProperties()
{
    p->properties[ PROTOCOLS ] = Supported | SupportingWMCheck;
    p->properties[ WINDOW_TYPES ] = NormalMask | DesktopMask | DockMask
                                    | ToolbarMask | MenuMask | DialogMask;
    p->properties[ STATES ] = Modal | Sticky | MaxVert | MaxHoriz | Shaded
                              | SkipTaskbar | StaysOnTop;
    p->properties[ PROTOCOLS2 ] = 0;
    p->properties[ ACTIONS ] = 0;
    p->client_properties[ PROTOCOLS ] = 0;
    p->client_properties[ WINDOW_TYPES ] = 0; // these two actually don't
    p->client_properties[ STATES ] = 0;       // make sense in client_properties
    p->client_properties[ PROTOCOLS2 ] = 0;
    p->client_properties[ ACTIONS ] = 0;
}

void NETRootInfo::activate()
{
    if (role == WindowManager) {
#ifdef    NETWMDEBUG
        fprintf(stderr,
                "NETRootInfo::activate: setting supported properties on root\n");
#endif
        setSupported();
    } else {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NETRootInfo::activate: updating client information\n");
#endif
        update(p->client_properties);
    }
}


void NETRootInfo::setClientList(Window *windows, unsigned int count)
{
    if (role != WindowManager) return;
    p->clients_count = count;
    delete [] p->clients;
    p->clients = nwindup(windows, count);
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::setClientList: setting list with %ld windows\n",
            p->clients_count);
#endif
    XChangeProperty(p->display, p->root, net_client_list, XA_WINDOW, 32,
                    PropModeReplace, (unsigned char *)p->clients,
                    p->clients_count);
}


void NETRootInfo::setClientListStacking(Window *windows, unsigned int count)
{
    if (role != WindowManager) return;
    p->stacking_count = count;
    delete [] p->stacking;
    p->stacking = nwindup(windows, count);
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::setClientListStacking: setting list with %ld windows\n",
            p->clients_count);
#endif
    XChangeProperty(p->display, p->root, net_client_list_stacking, XA_WINDOW, 32,
                    PropModeReplace, (unsigned char *) p->stacking,
                    p->stacking_count);
}


void NETRootInfo::setKDESystemTrayWindows(Window *windows, unsigned int count)
{
    if (role != WindowManager) return;
    p->kde_system_tray_windows_count = count;
    delete [] p->kde_system_tray_windows;
    p->kde_system_tray_windows = nwindup(windows, count);
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::setKDESystemTrayWindows: setting list with %ld windows\n",
            p->kde_system_tray_windows_count);
#endif
    XChangeProperty(p->display, p->root, kde_net_system_tray_windows, XA_WINDOW, 32,
                    PropModeReplace,
                    (unsigned char *) p->kde_system_tray_windows,
                    p->kde_system_tray_windows_count);
}


void NETRootInfo::setNumberOfDesktops(int numberOfDesktops)
{
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::setNumberOfDesktops: setting desktop count to %d (%s)\n",
            numberOfDesktops, (role == WindowManager) ? "WM" : "Client");
#endif
    if (role == WindowManager) {
        p->number_of_desktops = numberOfDesktops;
        long d = numberOfDesktops;
        XChangeProperty(p->display, p->root, net_number_of_desktops, XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *) &d, 1);
    } else {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_number_of_desktops;
        e.xclient.display = p->display;
        e.xclient.window = p->root;
        e.xclient.format = 32;
        e.xclient.data.l[0] = numberOfDesktops;
        e.xclient.data.l[1] = 0l;
        e.xclient.data.l[2] = 0l;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    }
}


void NETRootInfo::setCurrentDesktop(int desktop)
{
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::setCurrentDesktop: setting current desktop = %d (%s)\n",
            desktop, (role == WindowManager) ? "WM" : "Client");
#endif
    if (role == WindowManager) {
        p->current_desktop = desktop;
        long d = p->current_desktop - 1;
        XChangeProperty(p->display, p->root, net_current_desktop, XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *) &d, 1);
    } else {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_current_desktop;
        e.xclient.display = p->display;
        e.xclient.window = p->root;
        e.xclient.format = 32;
        e.xclient.data.l[0] = desktop - 1;
        e.xclient.data.l[1] = 0l;
        e.xclient.data.l[2] = 0l;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    }
}


void NETRootInfo::setDesktopName(int desktop, const char *desktopName)
{
    // allow setting desktop names even for non-existant desktops, see the spec, sect.3.7.
    if (desktop < 1) return;
    delete [] p->desktop_names[desktop - 1];
    p->desktop_names[desktop - 1] = nstrdup(desktopName);
    unsigned int i, proplen,
    num = ((p->number_of_desktops > p->desktop_names.size()) ?
           p->number_of_desktops : p->desktop_names.size());
    for (i = 0, proplen = 0; i < num; i++)
        proplen += (p->desktop_names[i] != 0 ? strlen(p->desktop_names[i]) + 1 : 1);
    char *prop = new char[proplen], *propp = prop;
    for (i = 0; i < num; i++)
        if (p->desktop_names[i]) {
            strcpy(propp, p->desktop_names[i]);
            propp += strlen(p->desktop_names[i]) + 1;
        } else
            *propp++ = '\0';
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::setDesktopName(%d, '%s')\n"
            "NETRootInfo::setDesktopName: total property length = %d",
            desktop, desktopName, proplen);
#endif
    XChangeProperty(p->display, p->root, net_desktop_names, UTF8_STRING, 8,
                    PropModeReplace, (unsigned char *) prop, proplen);
    delete [] prop;
}


void NETRootInfo::setDesktopGeometry(int , const NETSize &geometry)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::setDesktopGeometry(-- , { %d, %d }) (%s)\n",
            geometry.width, geometry.height, (role == WindowManager) ? "WM" : "Client");
#endif
    if (role == WindowManager) {
        p->geometry = geometry;
        long data[2];
        data[0] = p->geometry.width;
        data[1] = p->geometry.height;
        XChangeProperty(p->display, p->root, net_desktop_geometry, XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *) data, 2);
    } else {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_desktop_geometry;
        e.xclient.display = p->display;
        e.xclient.window = p->root;
        e.xclient.format = 32;
        e.xclient.data.l[0] = geometry.width;
        e.xclient.data.l[1] = geometry.height;
        e.xclient.data.l[2] = 0l;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    }
}


void NETRootInfo::setDesktopViewport(int desktop, const NETPoint &viewport)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::setDesktopViewport(%d, { %d, %d }) (%s)\n",
            desktop, viewport.x, viewport.y, (role == WindowManager) ? "WM" : "Client");
#endif
    if (desktop < 1) return;
    if (role == WindowManager) {
        p->viewport[desktop - 1] = viewport;
        int d, i, l;
        l = p->number_of_desktops * 2;
        long *data = new long[l];
        for (d = 0, i = 0; d < p->number_of_desktops; d++) {
            data[i++] = p->viewport[d].x;
            data[i++] = p->viewport[d].y;
        }
        XChangeProperty(p->display, p->root, net_desktop_viewport, XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *) data, l);
        delete [] data;
    } else {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_desktop_viewport;
        e.xclient.display = p->display;
        e.xclient.window = p->root;
        e.xclient.format = 32;
        e.xclient.data.l[0] = viewport.x;
        e.xclient.data.l[1] = viewport.y;
        e.xclient.data.l[2] = 0l;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    }
}


void NETRootInfo::setSupported()
{
    if (role != WindowManager) {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NETRootInfo::setSupported - role != WindowManager\n");
#endif
        return;
    }
    Atom atoms[netAtomCount];
    int pnum = 2;
    // Root window properties/messages
    atoms[0] = net_supported;
    atoms[1] = net_supporting_wm_check;
    if (p->properties[ PROTOCOLS ] & ClientList)
        atoms[pnum++] = net_client_list;
    if (p->properties[ PROTOCOLS ] & ClientListStacking)
        atoms[pnum++] = net_client_list_stacking;
    if (p->properties[ PROTOCOLS ] & NumberOfDesktops)
        atoms[pnum++] = net_number_of_desktops;
    if (p->properties[ PROTOCOLS ] & DesktopGeometry)
        atoms[pnum++] = net_desktop_geometry;
    if (p->properties[ PROTOCOLS ] & DesktopViewport)
        atoms[pnum++] = net_desktop_viewport;
    if (p->properties[ PROTOCOLS ] & CurrentDesktop)
        atoms[pnum++] = net_current_desktop;
    if (p->properties[ PROTOCOLS ] & DesktopNames)
        atoms[pnum++] = net_desktop_names;
    if (p->properties[ PROTOCOLS ] & ActiveWindow)
        atoms[pnum++] = net_active_window;
    if (p->properties[ PROTOCOLS ] & WorkArea)
        atoms[pnum++] = net_workarea;
    if (p->properties[ PROTOCOLS ] & VirtualRoots)
        atoms[pnum++] = net_virtual_roots;
    if (p->properties[ PROTOCOLS ] & CloseWindow)
        atoms[pnum++] = net_close_window;
    if (p->properties[ PROTOCOLS2 ] & WM2RestackWindow)
        atoms[pnum++] = net_restack_window;
    if (p->properties[ PROTOCOLS2 ] & WM2ShowingDesktop)
        atoms[pnum++] = net_showing_desktop;
    // Application window properties/messages
    if (p->properties[ PROTOCOLS ] & WMMoveResize)
        atoms[pnum++] = net_wm_moveresize;
    if (p->properties[ PROTOCOLS2 ] & WM2MoveResizeWindow)
        atoms[pnum++] = net_moveresize_window;
    if (p->properties[ PROTOCOLS ] & WMName)
        atoms[pnum++] = net_wm_name;
    if (p->properties[ PROTOCOLS ] & WMVisibleName)
        atoms[pnum++] = net_wm_visible_name;
    if (p->properties[ PROTOCOLS ] & WMIconName)
        atoms[pnum++] = net_wm_icon_name;
    if (p->properties[ PROTOCOLS ] & WMVisibleIconName)
        atoms[pnum++] = net_wm_visible_icon_name;
    if (p->properties[ PROTOCOLS ] & WMDesktop)
        atoms[pnum++] = net_wm_desktop;
    if (p->properties[ PROTOCOLS ] & WMWindowType) {
        atoms[pnum++] = net_wm_window_type;
        // Application window types
        if (p->properties[ WINDOW_TYPES ] & NormalMask)
            atoms[pnum++] = net_wm_window_type_normal;
        if (p->properties[ WINDOW_TYPES ] & DesktopMask)
            atoms[pnum++] = net_wm_window_type_desktop;
        if (p->properties[ WINDOW_TYPES ] & DockMask)
            atoms[pnum++] = net_wm_window_type_dock;
        if (p->properties[ WINDOW_TYPES ] & ToolbarMask)
            atoms[pnum++] = net_wm_window_type_toolbar;
        if (p->properties[ WINDOW_TYPES ] & MenuMask)
            atoms[pnum++] = net_wm_window_type_menu;
        if (p->properties[ WINDOW_TYPES ] & DialogMask)
            atoms[pnum++] = net_wm_window_type_dialog;
        if (p->properties[ WINDOW_TYPES ] & UtilityMask)
            atoms[pnum++] = net_wm_window_type_utility;
        if (p->properties[ WINDOW_TYPES ] & SplashMask)
            atoms[pnum++] = net_wm_window_type_splash;
        // KDE extensions
        if (p->properties[ WINDOW_TYPES ] & OverrideMask)
            atoms[pnum++] = kde_net_wm_window_type_override;
        if (p->properties[ WINDOW_TYPES ] & TopMenuMask)
            atoms[pnum++] = kde_net_wm_window_type_topmenu;
    }
    if (p->properties[ PROTOCOLS ] & WMState) {
        atoms[pnum++] = net_wm_state;
        // Application window states
        if (p->properties[ STATES ] & Modal)
            atoms[pnum++] = net_wm_state_modal;
        if (p->properties[ STATES ] & Sticky)
            atoms[pnum++] = net_wm_state_sticky;
        if (p->properties[ STATES ] & MaxVert)
            atoms[pnum++] = net_wm_state_max_vert;
        if (p->properties[ STATES ] & MaxHoriz)
            atoms[pnum++] = net_wm_state_max_horiz;
        if (p->properties[ STATES ] & Shaded)
            atoms[pnum++] = net_wm_state_shaded;
        if (p->properties[ STATES ] & SkipTaskbar)
            atoms[pnum++] = net_wm_state_skip_taskbar;
        if (p->properties[ STATES ] & SkipPager)
            atoms[pnum++] = net_wm_state_skip_pager;
        if (p->properties[ STATES ] & Hidden)
            atoms[pnum++] = net_wm_state_hidden;
        if (p->properties[ STATES ] & FullScreen)
            atoms[pnum++] = net_wm_state_fullscreen;
        if (p->properties[ STATES ] & KeepAbove)
            atoms[pnum++] = net_wm_state_above;
        if (p->properties[ STATES ] & KeepBelow)
            atoms[pnum++] = net_wm_state_below;
        if (p->properties[ STATES ] & DemandsAttention)
            atoms[pnum++] = net_wm_state_demands_attention;
        if (p->properties[ STATES ] & StaysOnTop)
            atoms[pnum++] = net_wm_state_stays_on_top;
    }
    if (p->properties[ PROTOCOLS ] & WMStrut)
        atoms[pnum++] = net_wm_strut;
    if (p->properties[ PROTOCOLS2 ] & WM2ExtendedStrut)
        atoms[pnum++] = net_wm_extended_strut;
    if (p->properties[ PROTOCOLS ] & WMIconGeometry)
        atoms[pnum++] = net_wm_icon_geometry;
    if (p->properties[ PROTOCOLS ] & WMIcon)
        atoms[pnum++] = net_wm_icon;
    if (p->properties[ PROTOCOLS ] & WMPid)
        atoms[pnum++] = net_wm_pid;
    if (p->properties[ PROTOCOLS ] & WMHandledIcons)
        atoms[pnum++] = net_wm_handled_icons;
    if (p->properties[ PROTOCOLS ] & WMPing)
        atoms[pnum++] = net_wm_ping;
    if (p->properties[ PROTOCOLS2 ] & WM2TakeActivity)
        atoms[pnum++] = net_wm_take_activity;
    if (p->properties[ PROTOCOLS2 ] & WM2UserTime)
        atoms[pnum++] = net_wm_user_time;
    if (p->properties[ PROTOCOLS2 ] & WM2StartupId)
        atoms[pnum++] = net_startup_id;
    if (p->properties[ PROTOCOLS2 ] & WM2AllowedActions) {
        atoms[pnum++] = net_wm_allowed_actions;
        // Actions
        if (p->properties[ ACTIONS ] & ActionMove)
            atoms[pnum++] = net_wm_action_move;
        if (p->properties[ ACTIONS ] & ActionResize)
            atoms[pnum++] = net_wm_action_resize;
        if (p->properties[ ACTIONS ] & ActionMinimize)
            atoms[pnum++] = net_wm_action_minimize;
        if (p->properties[ ACTIONS ] & ActionShade)
            atoms[pnum++] = net_wm_action_shade;
        if (p->properties[ ACTIONS ] & ActionStick)
            atoms[pnum++] = net_wm_action_stick;
        if (p->properties[ ACTIONS ] & ActionMaxVert)
            atoms[pnum++] = net_wm_action_max_vert;
        if (p->properties[ ACTIONS ] & ActionMaxHoriz)
            atoms[pnum++] = net_wm_action_max_horiz;
        if (p->properties[ ACTIONS ] & ActionFullScreen)
            atoms[pnum++] = net_wm_action_fullscreen;
        if (p->properties[ ACTIONS ] & ActionChangeDesktop)
            atoms[pnum++] = net_wm_action_change_desk;
        if (p->properties[ ACTIONS ] & ActionClose)
            atoms[pnum++] = net_wm_action_close;
    }
    // KDE specific extensions
    if (p->properties[ PROTOCOLS ] & KDESystemTrayWindows)
        atoms[pnum++] = kde_net_system_tray_windows;
    if (p->properties[ PROTOCOLS ] & WMKDESystemTrayWinFor)
        atoms[pnum++] = kde_net_wm_system_tray_window_for;
    if (p->properties[ PROTOCOLS ] & WMFrameExtents) {
        atoms[pnum++] = net_frame_extents;
        atoms[pnum++] = kde_net_wm_frame_strut;
    }
    if (p->properties[ PROTOCOLS2 ] & WM2KDETemporaryRules)
        atoms[pnum++] = kde_net_wm_temporary_rules;
    XChangeProperty(p->display, p->root, net_supported, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *) atoms, pnum);
    XChangeProperty(p->display, p->root, net_supporting_wm_check, XA_WINDOW, 32,
                    PropModeReplace, (unsigned char *) &(p->supportwindow), 1);
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::setSupported: _NET_SUPPORTING_WM_CHECK = 0x%lx on 0x%lx\n"
            "                         : _NET_WM_NAME = '%s' on 0x%lx\n",
            p->supportwindow, p->supportwindow, p->name, p->supportwindow);
#endif
    XChangeProperty(p->display, p->supportwindow, net_supporting_wm_check,
                    XA_WINDOW, 32, PropModeReplace,
                    (unsigned char *) &(p->supportwindow), 1);
    XChangeProperty(p->display, p->supportwindow, net_wm_name, UTF8_STRING, 8,
                    PropModeReplace, (unsigned char *) p->name,
                    strlen(p->name));
}

void NETRootInfo::updateSupportedProperties(Atom atom)
{
    if (atom == net_supported)
        p->properties[ PROTOCOLS ] |= Supported;
    else if (atom == net_supporting_wm_check)
        p->properties[ PROTOCOLS ] |= SupportingWMCheck;
    else if (atom == net_client_list)
        p->properties[ PROTOCOLS ] |= ClientList;
    else if (atom == net_client_list_stacking)
        p->properties[ PROTOCOLS ] |= ClientListStacking;
    else if (atom == net_number_of_desktops)
        p->properties[ PROTOCOLS ] |= NumberOfDesktops;
    else if (atom == net_desktop_geometry)
        p->properties[ PROTOCOLS ] |= DesktopGeometry;
    else if (atom == net_desktop_viewport)
        p->properties[ PROTOCOLS ] |= DesktopViewport;
    else if (atom == net_current_desktop)
        p->properties[ PROTOCOLS ] |= CurrentDesktop;
    else if (atom == net_desktop_names)
        p->properties[ PROTOCOLS ] |= DesktopNames;
    else if (atom == net_active_window)
        p->properties[ PROTOCOLS ] |= ActiveWindow;
    else if (atom == net_workarea)
        p->properties[ PROTOCOLS ] |= WorkArea;
    else if (atom == net_virtual_roots)
        p->properties[ PROTOCOLS ] |= VirtualRoots;
    else if (atom == net_close_window)
        p->properties[ PROTOCOLS ] |= CloseWindow;
    else if (atom == net_restack_window)
        p->properties[ PROTOCOLS2 ] |= WM2RestackWindow;
    else if (atom == net_showing_desktop)
        p->properties[ PROTOCOLS2 ] |= WM2ShowingDesktop;
    // Application window properties/messages
    else if (atom == net_wm_moveresize)
        p->properties[ PROTOCOLS ] |= WMMoveResize;
    else if (atom == net_moveresize_window)
        p->properties[ PROTOCOLS2 ] |= WM2MoveResizeWindow;
    else if (atom == net_wm_name)
        p->properties[ PROTOCOLS ] |= WMName;
    else if (atom == net_wm_visible_name)
        p->properties[ PROTOCOLS ] |= WMVisibleName;
    else if (atom == net_wm_icon_name)
        p->properties[ PROTOCOLS ] |= WMIconName;
    else if (atom == net_wm_visible_icon_name)
        p->properties[ PROTOCOLS ] |= WMVisibleIconName;
    else if (atom == net_wm_desktop)
        p->properties[ PROTOCOLS ] |= WMDesktop;
    else if (atom == net_wm_window_type)
        p->properties[ PROTOCOLS ] |= WMWindowType;
    // Application window types
    else if (atom == net_wm_window_type_normal)
        p->properties[ WINDOW_TYPES ] |= NormalMask;
    else if (atom == net_wm_window_type_desktop)
        p->properties[ WINDOW_TYPES ] |= DesktopMask;
    else if (atom == net_wm_window_type_dock)
        p->properties[ WINDOW_TYPES ] |= DockMask;
    else if (atom == net_wm_window_type_toolbar)
        p->properties[ WINDOW_TYPES ] |= ToolbarMask;
    else if (atom == net_wm_window_type_menu)
        p->properties[ WINDOW_TYPES ] |= MenuMask;
    else if (atom == net_wm_window_type_dialog)
        p->properties[ WINDOW_TYPES ] |= DialogMask;
    else if (atom == net_wm_window_type_utility)
        p->properties[ WINDOW_TYPES ] |= UtilityMask;
    else if (atom == net_wm_window_type_splash)
        p->properties[ WINDOW_TYPES ] |= SplashMask;
    // KDE extensions
    else if (atom == kde_net_wm_window_type_override)
        p->properties[ WINDOW_TYPES ] |= OverrideMask;
    else if (atom == kde_net_wm_window_type_topmenu)
        p->properties[ WINDOW_TYPES ] |= TopMenuMask;
    else if (atom == net_wm_state)
        p->properties[ PROTOCOLS ] |= WMState;
    // Application window states
    else if (atom == net_wm_state_modal)
        p->properties[ STATES ] |= Modal;
    else if (atom == net_wm_state_sticky)
        p->properties[ STATES ] |= Sticky;
    else if (atom == net_wm_state_max_vert)
        p->properties[ STATES ] |= MaxVert;
    else if (atom == net_wm_state_max_horiz)
        p->properties[ STATES ] |= MaxHoriz;
    else if (atom == net_wm_state_shaded)
        p->properties[ STATES ] |= Shaded;
    else if (atom == net_wm_state_skip_taskbar)
        p->properties[ STATES ] |= SkipTaskbar;
    else if (atom == net_wm_state_skip_pager)
        p->properties[ STATES ] |= SkipPager;
    else if (atom == net_wm_state_hidden)
        p->properties[ STATES ] |= Hidden;
    else if (atom == net_wm_state_fullscreen)
        p->properties[ STATES ] |= FullScreen;
    else if (atom == net_wm_state_above)
        p->properties[ STATES ] |= KeepAbove;
    else if (atom == net_wm_state_below)
        p->properties[ STATES ] |= KeepBelow;
    else if (atom == net_wm_state_demands_attention)
        p->properties[ STATES ] |= DemandsAttention;
    else if (atom == net_wm_state_stays_on_top)
        p->properties[ STATES ] |= StaysOnTop;
    else if (atom == net_wm_strut)
        p->properties[ PROTOCOLS ] |= WMStrut;
    else if (atom == net_wm_extended_strut)
        p->properties[ PROTOCOLS2 ] |= WM2ExtendedStrut;
    else if (atom == net_wm_icon_geometry)
        p->properties[ PROTOCOLS ] |= WMIconGeometry;
    else if (atom == net_wm_icon)
        p->properties[ PROTOCOLS ] |= WMIcon;
    else if (atom == net_wm_pid)
        p->properties[ PROTOCOLS ] |= WMPid;
    else if (atom == net_wm_handled_icons)
        p->properties[ PROTOCOLS ] |= WMHandledIcons;
    else if (atom == net_wm_ping)
        p->properties[ PROTOCOLS ] |= WMPing;
    else if (atom == net_wm_take_activity)
        p->properties[ PROTOCOLS2 ] |= WM2TakeActivity;
    else if (atom == net_wm_user_time)
        p->properties[ PROTOCOLS2 ] |= WM2UserTime;
    else if (atom == net_startup_id)
        p->properties[ PROTOCOLS2 ] |= WM2StartupId;
    else if (atom == net_wm_allowed_actions)
        p->properties[ PROTOCOLS2 ] |= WM2AllowedActions;
    // Actions
    else if (atom == net_wm_action_move)
        p->properties[ ACTIONS ] |= ActionMove;
    else if (atom == net_wm_action_resize)
        p->properties[ ACTIONS ] |= ActionResize;
    else if (atom == net_wm_action_minimize)
        p->properties[ ACTIONS ] |= ActionMinimize;
    else if (atom == net_wm_action_shade)
        p->properties[ ACTIONS ] |= ActionShade;
    else if (atom == net_wm_action_stick)
        p->properties[ ACTIONS ] |= ActionStick;
    else if (atom == net_wm_action_max_vert)
        p->properties[ ACTIONS ] |= ActionMaxVert;
    else if (atom == net_wm_action_max_horiz)
        p->properties[ ACTIONS ] |= ActionMaxHoriz;
    else if (atom == net_wm_action_fullscreen)
        p->properties[ ACTIONS ] |= ActionFullScreen;
    else if (atom == net_wm_action_change_desk)
        p->properties[ ACTIONS ] |= ActionChangeDesktop;
    else if (atom == net_wm_action_close)
        p->properties[ ACTIONS ] |= ActionClose;
    // KDE specific extensions
    else if (atom == kde_net_system_tray_windows)
        p->properties[ PROTOCOLS ] |= KDESystemTrayWindows;
    else if (atom == kde_net_wm_system_tray_window_for)
        p->properties[ PROTOCOLS ] |= WMKDESystemTrayWinFor;
    else if (atom == net_frame_extents)
        p->properties[ PROTOCOLS ] |= WMFrameExtents;
    else if (atom == kde_net_wm_frame_strut)
        p->properties[ PROTOCOLS ] |= WMKDEFrameStrut;
    else if (atom == kde_net_wm_temporary_rules)
        p->properties[ PROTOCOLS2 ] |= WM2KDETemporaryRules;
}

extern Time qt_x_user_time;
void NETRootInfo::setActiveWindow(Window window)
{
    setActiveWindow(window, FromUnknown, 0, None);
}

void NETRootInfo::setActiveWindow(Window window, NET::RequestSource src,
                                  Time timestamp, Window active_window)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::setActiveWindow(0x%lx) (%s)\n",
            window, (role == WindowManager) ? "WM" : "Client");
#endif
    if (role == WindowManager) {
        p->active = window;
        XChangeProperty(p->display, p->root, net_active_window, XA_WINDOW, 32,
                        PropModeReplace, (unsigned char *) &(p->active), 1);
    } else {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_active_window;
        e.xclient.display = p->display;
        e.xclient.window = window;
        e.xclient.format = 32;
        e.xclient.data.l[0] = src;
        e.xclient.data.l[1] = timestamp;
        e.xclient.data.l[2] = active_window;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    }
}


void NETRootInfo::setWorkArea(int desktop, const NETRect &workarea)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::setWorkArea(%d, { %d, %d, %d, %d }) (%s)\n",
            desktop, workarea.pos.x, workarea.pos.y, workarea.size.width, workarea.size.height,
            (role == WindowManager) ? "WM" : "Client");
#endif
    if (role != WindowManager || desktop < 1) return;
    p->workarea[desktop - 1] = workarea;
    long *wa = new long[p->number_of_desktops * 4];
    int i, o;
    for (i = 0, o = 0; i < p->number_of_desktops; i++) {
        wa[o++] = p->workarea[i].pos.x;
        wa[o++] = p->workarea[i].pos.y;
        wa[o++] = p->workarea[i].size.width;
        wa[o++] = p->workarea[i].size.height;
    }
    XChangeProperty(p->display, p->root, net_workarea, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) wa,
                    p->number_of_desktops * 4);
    delete [] wa;
}


void NETRootInfo::setVirtualRoots(Window *windows, unsigned int count)
{
    if (role != WindowManager) return;
    p->virtual_roots_count = count;
    p->virtual_roots = windows;
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETRootInfo::setVirtualRoots: setting list with %ld windows\n",
            p->virtual_roots_count);
#endif
    XChangeProperty(p->display, p->root, net_virtual_roots, XA_WINDOW, 32,
                    PropModeReplace, (unsigned char *) p->virtual_roots,
                    p->virtual_roots_count);
}


void NETRootInfo::setShowingDesktop(bool showing)
{
    if (role == WindowManager) {
        long d = p->showing_desktop = showing;
        XChangeProperty(p->display, p->root, net_showing_desktop, XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *) &d, 1);
    } else {
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_showing_desktop;
        e.xclient.display = p->display;
        e.xclient.window = 0;
        e.xclient.format = 32;
        e.xclient.data.l[0] = showing ? 1 : 0;
        e.xclient.data.l[1] = 0;
        e.xclient.data.l[2] = 0;
        e.xclient.data.l[3] = 0;
        e.xclient.data.l[4] = 0;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    }
}


bool NETRootInfo::showingDesktop() const
{
    return p->showing_desktop;
}


void NETRootInfo::closeWindowRequest(Window window)
{
#ifdef    NETWMDEBUG
    fprintf(stderr, "NETRootInfo::closeWindowRequest: requesting close for 0x%lx\n",
            window);
#endif
    XEvent e;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = net_close_window;
    e.xclient.display = p->display;
    e.xclient.window = window;
    e.xclient.format = 32;
    e.xclient.data.l[0] = 0l;
    e.xclient.data.l[1] = 0l;
    e.xclient.data.l[2] = 0l;
    e.xclient.data.l[3] = 0l;
    e.xclient.data.l[4] = 0l;
    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
}


void NETRootInfo::moveResizeRequest(Window window, int x_root, int y_root,
                                    Direction direction)
{
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::moveResizeRequest: requesting resize/move for 0x%lx (%d, %d, %d)\n",
            window, x_root, y_root, direction);
#endif
    XEvent e;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = net_wm_moveresize;
    e.xclient.display = p->display;
    e.xclient.window = window,
                       e.xclient.format = 32;
    e.xclient.data.l[0] = x_root;
    e.xclient.data.l[1] = y_root;
    e.xclient.data.l[2] = direction;
    e.xclient.data.l[3] = 0l;
    e.xclient.data.l[4] = 0l;
    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
}

void NETRootInfo::moveResizeWindowRequest(Window window, int flags, int x, int y, int width, int height)
{
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::moveResizeWindowRequest: resizing/moving 0x%lx (%d, %d, %d, %d, %d)\n",
            window, flags, x, y, width, height);
#endif
    XEvent e;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = net_moveresize_window;
    e.xclient.display = p->display;
    e.xclient.window = window,
                       e.xclient.format = 32;
    e.xclient.data.l[0] = flags;
    e.xclient.data.l[1] = x;
    e.xclient.data.l[2] = y;
    e.xclient.data.l[3] = width;
    e.xclient.data.l[4] = height;
    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
}

void NETRootInfo::restackRequest(Window window, Window above, int detail)
{
    restackRequest(window, FromTool, above, detail, 0);
}

void NETRootInfo::restackRequest(Window window, RequestSource src, Window above, int detail, Time timestamp)
{
#ifdef    NETWMDEBUG
    fprintf(stderr,
            "NETRootInfo::restackRequest: requesting restack for 0x%lx (%lx, %d)\n",
            window, above, detail);
#endif
    XEvent e;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = net_restack_window;
    e.xclient.display = p->display;
    e.xclient.window = window,
                       e.xclient.format = 32;
    e.xclient.data.l[0] = src;
    e.xclient.data.l[1] = above;
    e.xclient.data.l[2] = detail;
    e.xclient.data.l[3] = timestamp;
    e.xclient.data.l[4] = 0l;
    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
}

void NETRootInfo2::sendPing(Window window, Time timestamp)
{
    if (role != WindowManager) return;
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETRootInfo2::setPing: window 0x%lx, timestamp %lu\n",
            window, timestamp);
#endif
    XEvent e;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = wm_protocols;
    e.xclient.display = p->display;
    e.xclient.window = window,
                       e.xclient.format = 32;
    e.xclient.data.l[0] = net_wm_ping;
    e.xclient.data.l[1] = timestamp;
    e.xclient.data.l[2] = window;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;
    XSendEvent(p->display, window, False, 0, &e);
}

void NETRootInfo3::takeActivity(Window window, Time timestamp, long flags)
{
    if (role != WindowManager) return;
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETRootInfo2::takeActivity: window 0x%lx, timestamp %lu, flags 0x%lx\n",
            window, timestamp, flags);
#endif
    XEvent e;
    e.xclient.type = ClientMessage;
    e.xclient.message_type = wm_protocols;
    e.xclient.display = p->display;
    e.xclient.window = window,
                       e.xclient.format = 32;
    e.xclient.data.l[0] = net_wm_take_activity;
    e.xclient.data.l[1] = timestamp;
    e.xclient.data.l[2] = window;
    e.xclient.data.l[3] = flags;
    e.xclient.data.l[4] = 0;
    XSendEvent(p->display, window, False, 0, &e);
}



// assignment operator

const NETRootInfo &NETRootInfo::operator=(const NETRootInfo & rootinfo)
{
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETRootInfo::operator=()\n");
#endif
    if (p != rootinfo.p) {
        refdec_nri(p);
        if (! p->ref) delete p;
    }
    p = rootinfo.p;
    role = rootinfo.role;
    p->ref++;
    return *this;
}

unsigned long NETRootInfo::event(XEvent *ev)
{
    unsigned long props[ 1 ];
    event(ev, props, 1);
    return props[ 0 ];
}

void NETRootInfo::event(XEvent *event, unsigned long* properties, int properties_size)
{
    unsigned long props[ PROPERTIES_SIZE ] = { 0, 0, 0, 0, 0 };
    assert(PROPERTIES_SIZE == 5);   // add elements above
    unsigned long& dirty = props[ PROTOCOLS ];
    unsigned long& dirty2 = props[ PROTOCOLS2 ];
    bool do_update = false;
    // the window manager will be interested in client messages... no other
    // client should get these messages
    if (role == WindowManager && event->type == ClientMessage &&
            event->xclient.format == 32) {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NETRootInfo::event: handling ClientMessage event\n");
#endif
        if (event->xclient.message_type == net_number_of_desktops) {
            dirty = NumberOfDesktops;
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: changeNumberOfDesktops(%ld)\n",
                    event->xclient.data.l[0]);
#endif
            changeNumberOfDesktops(event->xclient.data.l[0]);
        } else if (event->xclient.message_type == net_desktop_geometry) {
            dirty = DesktopGeometry;
            NETSize sz;
            sz.width = event->xclient.data.l[0];
            sz.height = event->xclient.data.l[1];
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: changeDesktopGeometry(-- , { %d, %d })\n",
                    sz.width, sz.height);
#endif
            changeDesktopGeometry(~0, sz);
        } else if (event->xclient.message_type == net_desktop_viewport) {
            dirty = DesktopViewport;
            NETPoint pt;
            pt.x = event->xclient.data.l[0];
            pt.y = event->xclient.data.l[1];
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: changeDesktopViewport(%d, { %d, %d })\n",
                    p->current_desktop, pt.x, pt.y);
#endif
            changeDesktopViewport(p->current_desktop, pt);
        } else if (event->xclient.message_type == net_current_desktop) {
            dirty = CurrentDesktop;
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: changeCurrentDesktop(%ld)\n",
                    event->xclient.data.l[0] + 1);
#endif
            changeCurrentDesktop(event->xclient.data.l[0] + 1);
        } else if (event->xclient.message_type == net_active_window) {
            dirty = ActiveWindow;
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: changeActiveWindow(0x%lx)\n",
                    event->xclient.window);
#endif
            changeActiveWindow(event->xclient.window);
            if (NETRootInfo2* this2 = dynamic_cast< NETRootInfo2* >(this)) {
                RequestSource src = FromUnknown;
                Time timestamp = CurrentTime;
                Window active_window = None;
                // make sure there aren't unknown values
                if (event->xclient.data.l[0] >= FromUnknown
                        && event->xclient.data.l[0] <= FromTool) {
                    src = static_cast< RequestSource >(event->xclient.data.l[0]);
                    timestamp = event->xclient.data.l[1];
                    active_window = event->xclient.data.l[2];
                }
                this2->changeActiveWindow(event->xclient.window, src, timestamp, active_window);
            }
        } else if (event->xclient.message_type == net_wm_moveresize) {
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: moveResize(%ld, %ld, %ld, %ld)\n",
                    event->xclient.window,
                    event->xclient.data.l[0],
                    event->xclient.data.l[1],
                    event->xclient.data.l[2]
                   );
#endif
            moveResize(event->xclient.window,
                       event->xclient.data.l[0],
                       event->xclient.data.l[1],
                       event->xclient.data.l[2]);
        } else if (event->xclient.message_type == net_moveresize_window) {
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: moveResizeWindow(%ld, %ld, %ld, %ld, %ld, %ld)\n",
                    event->xclient.window,
                    event->xclient.data.l[0],
                    event->xclient.data.l[1],
                    event->xclient.data.l[2],
                    event->xclient.data.l[3],
                    event->xclient.data.l[4]
                   );
#endif
            if (NETRootInfo2* this2 = dynamic_cast< NETRootInfo2* >(this))
                this2->moveResizeWindow(event->xclient.window,
                                        event->xclient.data.l[0],
                                        event->xclient.data.l[1],
                                        event->xclient.data.l[2],
                                        event->xclient.data.l[3],
                                        event->xclient.data.l[4]);
        } else if (event->xclient.message_type == net_close_window) {
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: closeWindow(0x%lx)\n",
                    event->xclient.window);
#endif
            closeWindow(event->xclient.window);
        } else if (event->xclient.message_type == net_restack_window) {
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: restackWindow(0x%lx)\n",
                    event->xclient.window);
#endif
            if (NETRootInfo3* this3 = dynamic_cast< NETRootInfo3* >(this)) {
                RequestSource src = FromUnknown;
                Time timestamp = CurrentTime;
                // make sure there aren't unknown values
                if (event->xclient.data.l[0] >= FromUnknown
                        && event->xclient.data.l[0] <= FromTool) {
                    src = static_cast< RequestSource >(event->xclient.data.l[0]);
                    timestamp = event->xclient.data.l[3];
                }
                this3->restackWindow(event->xclient.window, src,
                                     event->xclient.data.l[1], event->xclient.data.l[2], timestamp);
            } else if (NETRootInfo2* this2 = dynamic_cast< NETRootInfo2* >(this))
                this2->restackWindow(event->xclient.window,
                                     event->xclient.data.l[1], event->xclient.data.l[2]);
        } else if (event->xclient.message_type == wm_protocols
                   && (Atom)event->xclient.data.l[ 0 ] == net_wm_ping) {
            dirty = WMPing;
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo2::event: gotPing(0x%lx,%lu)\n",
                    event->xclient.window, event->xclient.data.l[1]);
#endif
            if (NETRootInfo2* this2 = dynamic_cast< NETRootInfo2* >(this))
                this2->gotPing(event->xclient.data.l[2], event->xclient.data.l[1]);
        } else if (event->xclient.message_type == wm_protocols
                   && (Atom)event->xclient.data.l[ 0 ] == net_wm_take_activity) {
            dirty2 = WM2TakeActivity;
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo2::event: gotTakeActivity(0x%lx,%lu,0x%lx)\n",
                    event->xclient.window, event->xclient.data.l[1], event->xclient.data.l[3]);
#endif
            if (NETRootInfo3* this3 = dynamic_cast< NETRootInfo3* >(this))
                this3->gotTakeActivity(event->xclient.data.l[2], event->xclient.data.l[1],
                                       event->xclient.data.l[3]);
        } else if (event->xclient.message_type == net_showing_desktop) {
            dirty2 = WM2ShowingDesktop;
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: changeShowingDesktop(%ld)\n",
                    event->xclient.data.l[0]);
#endif
            if (NETRootInfo4* this4 = dynamic_cast< NETRootInfo4* >(this))
                this4->changeShowingDesktop(event->xclient.data.l[0]);
        }
    }
    if (event->type == PropertyNotify) {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NETRootInfo::event: handling PropertyNotify event\n");
#endif
        XEvent pe = *event;
        Bool done = False;
        Bool compaction = False;
        while (! done) {
#ifdef   NETWMDEBUG
            fprintf(stderr, "NETRootInfo::event: loop fire\n");
#endif
            if (pe.xproperty.atom == net_client_list)
                dirty |= ClientList;
            else if (pe.xproperty.atom == net_client_list_stacking)
                dirty |= ClientListStacking;
            else if (pe.xproperty.atom == kde_net_system_tray_windows)
                dirty |= KDESystemTrayWindows;
            else if (pe.xproperty.atom == net_desktop_names)
                dirty |= DesktopNames;
            else if (pe.xproperty.atom == net_workarea)
                dirty |= WorkArea;
            else if (pe.xproperty.atom == net_number_of_desktops)
                dirty |= NumberOfDesktops;
            else if (pe.xproperty.atom == net_desktop_geometry)
                dirty |= DesktopGeometry;
            else if (pe.xproperty.atom == net_desktop_viewport)
                dirty |= DesktopViewport;
            else if (pe.xproperty.atom == net_current_desktop)
                dirty |= CurrentDesktop;
            else if (pe.xproperty.atom == net_active_window)
                dirty |= ActiveWindow;
            else if (pe.xproperty.atom == net_showing_desktop)
                dirty2 |= WM2ShowingDesktop;
            else {
#ifdef    NETWMDEBUG
                fprintf(stderr, "NETRootInfo::event: putting back event and breaking\n");
#endif
                if (compaction)
                    XPutBackEvent(p->display, &pe);
                break;
            }
            if (XCheckTypedWindowEvent(p->display, p->root, PropertyNotify, &pe))
                compaction = True;
            else
                break;
        }
        do_update = true;
    }
    if (do_update)
        update(props);
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETRootInfo::event: handled events, returning dirty = 0x%lx, 0x%lx\n",
            dirty, dirty2);
#endif
    if (properties_size > PROPERTIES_SIZE)
        properties_size = PROPERTIES_SIZE;
    for (int i = 0;
            i < properties_size;
            ++i)
        properties[ i ] = props[ i ];
}


// private functions to update the data we keep

void NETRootInfo::update(const unsigned long dirty_props[])
{
    Atom type_ret;
    int format_ret;
    unsigned char *data_ret;
    unsigned long nitems_ret, unused;
    unsigned long props[ PROPERTIES_SIZE ];
    for (int i = 0;
            i < PROPERTIES_SIZE;
            ++i)
        props[ i ] = dirty_props[ i ] & p->client_properties[ i ];
    const unsigned long& dirty = props[ PROTOCOLS ];
    const unsigned long& dirty2 = props[ PROTOCOLS2 ];
    if (dirty & Supported) {
        // only in Client mode
        for (int i = 0; i < PROPERTIES_SIZE; ++i)
            p->properties[ i ] = 0;
        if (XGetWindowProperty(p->display, p->root, net_supported,
                               0l, MAX_PROP_SIZE, False, XA_ATOM, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_ATOM && format_ret == 32) {
                Atom* atoms = (Atom*) data_ret;
                for (unsigned int i = 0;
                        i < nitems_ret;
                        ++i)
                    updateSupportedProperties(atoms[ i ]);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & ClientList) {
        bool read_ok = false;
        if (XGetWindowProperty(p->display, p->root, net_client_list,
                               0l, MAX_PROP_SIZE, False, XA_WINDOW, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32) {
                Window *wins = (Window *) data_ret;
                qsort(wins, nitems_ret, sizeof(Window), wcmp);
                if (p->clients) {
                    if (role == Client) {
                        unsigned long new_index = 0, old_index = 0;
                        unsigned long new_count = nitems_ret,
                                                  old_count = p->clients_count;
                        while (old_index < old_count || new_index < new_count) {
                            if (old_index == old_count) {
                                addClient(wins[new_index++]);
                            } else if (new_index == new_count) {
                                removeClient(p->clients[old_index++]);
                            } else {
                                if (p->clients[old_index] <
                                        wins[new_index]) {
                                    removeClient(p->clients[old_index++]);
                                } else if (wins[new_index] <
                                           p->clients[old_index]) {
                                    addClient(wins[new_index++]);
                                } else {
                                    new_index++;
                                    old_index++;
                                }
                            }
                        }
                    }
                    delete [] p->clients;
                } else {
#ifdef    NETWMDEBUG
                    fprintf(stderr, "NETRootInfo::update: client list null, creating\n");
#endif
                    unsigned long n;
                    for (n = 0; n < nitems_ret; n++) {
                        addClient(wins[n]);
                    }
                }
                p->clients_count = nitems_ret;
                p->clients = nwindup(wins, p->clients_count);
                read_ok = true;
            }
            if (data_ret)
                XFree(data_ret);
        }
        if (!read_ok) {
            for (unsigned int i = 0; i < p->clients_count; ++ i)
                removeClient(p->clients[i]);
            p->clients_count = 0;
            delete[] p->clients;
            p->clients = NULL;
        }
#ifdef    NETWMDEBUG
        fprintf(stderr, "NETRootInfo::update: client list updated (%ld clients)\n",
                p->clients_count);
#endif
    }
    if (dirty & KDESystemTrayWindows) {
        bool read_ok = false;
        if (XGetWindowProperty(p->display, p->root, kde_net_system_tray_windows,
                               0l, MAX_PROP_SIZE, False, XA_WINDOW, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32) {
                Window *wins = (Window *) data_ret;
                qsort(wins, nitems_ret, sizeof(Window), wcmp);
                if (p->kde_system_tray_windows) {
                    if (role == Client) {
                        unsigned long new_index = 0, new_count = nitems_ret;
                        unsigned long old_index = 0,
                                                  old_count = p->kde_system_tray_windows_count;
                        while (old_index < old_count || new_index < new_count) {
                            if (old_index == old_count) {
                                addSystemTrayWin(wins[new_index++]);
                            } else if (new_index == new_count) {
                                removeSystemTrayWin(p->kde_system_tray_windows[old_index++]);
                            } else {
                                if (p->kde_system_tray_windows[old_index] <
                                        wins[new_index]) {
                                    removeSystemTrayWin(p->kde_system_tray_windows[old_index++]);
                                } else if (wins[new_index] <
                                           p->kde_system_tray_windows[old_index]) {
                                    addSystemTrayWin(wins[new_index++]);
                                } else {
                                    new_index++;
                                    old_index++;
                                }
                            }
                        }
                    }
                } else {
                    unsigned long n;
                    for (n = 0; n < nitems_ret; n++) {
                        addSystemTrayWin(wins[n]);
                    }
                }
                p->kde_system_tray_windows_count = nitems_ret;
                delete [] p->kde_system_tray_windows;
                p->kde_system_tray_windows =
                    nwindup(wins, p->kde_system_tray_windows_count);
                read_ok = true;
            }
            if (data_ret)
                XFree(data_ret);
        }
        if (!read_ok) {
            for (unsigned int i = 0; i < p->kde_system_tray_windows_count; ++i)
                removeSystemTrayWin(p->kde_system_tray_windows[i]);
            p->kde_system_tray_windows_count = 0;
            delete [] p->kde_system_tray_windows;
            p->kde_system_tray_windows = NULL;
        }
    }
    if (dirty & ClientListStacking) {
        p->stacking_count = 0;
        delete[] p->stacking;
        p->stacking = NULL;
        if (XGetWindowProperty(p->display, p->root, net_client_list_stacking,
                               0, MAX_PROP_SIZE, False, XA_WINDOW, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32) {
                Window *wins = (Window *) data_ret;
                p->stacking_count = nitems_ret;
                p->stacking = nwindup(wins, p->stacking_count);
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: client stacking updated (%ld clients)\n",
                    p->stacking_count);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & NumberOfDesktops) {
        p->number_of_desktops = 0;
        if (XGetWindowProperty(p->display, p->root, net_number_of_desktops,
                               0l, 1l, False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 && nitems_ret == 1) {
                p->number_of_desktops = *((long *) data_ret);
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: number of desktops = %d\n",
                    p->number_of_desktops);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & DesktopGeometry) {
        p->geometry = p->rootSize;
        if (XGetWindowProperty(p->display, p->root, net_desktop_geometry,
                               0l, 2l, False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == 2) {
                long *data = (long *) data_ret;
                p->geometry.width  = data[0];
                p->geometry.height = data[1];
#ifdef    NETWMDEBUG
                fprintf(stderr, "NETRootInfo::update: desktop geometry updated\n");
#endif
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & DesktopViewport) {
        for (int i = 0; i < p->viewport.size(); i++)
            p->viewport[i].x = p->viewport[i].y = 0;
        if (XGetWindowProperty(p->display, p->root, net_desktop_viewport,
                               0l, 2l, False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == 2) {
                long *data = (long *) data_ret;
                int d, i, n;
                n = nitems_ret / 2;
                for (d = 0, i = 0; d < n; d++) {
                    p->viewport[d].x = data[i++];
                    p->viewport[d].y = data[i++];
                }
#ifdef    NETWMDEBUG
                fprintf(stderr,
                        "NETRootInfo::update: desktop viewport array updated (%d entries)\n",
                        p->viewport.size());
                if (nitems_ret % 2 != 0) {
                    fprintf(stderr,
                            "NETRootInfo::update(): desktop viewport array "
                            "size not a multiple of 2\n");
                }
#endif
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & CurrentDesktop) {
        p->current_desktop = 0;
        if (XGetWindowProperty(p->display, p->root, net_current_desktop,
                               0l, 1l, False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 && nitems_ret == 1) {
                p->current_desktop = *((long *) data_ret) + 1;
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: current desktop = %d\n",
                    p->current_desktop);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & DesktopNames) {
        for (int i = 0; i < p->desktop_names.size(); ++i)
            delete[] p->desktop_names[ i ];
        p->desktop_names.reset();
        if (XGetWindowProperty(p->display, p->root, net_desktop_names,
                               0l, MAX_PROP_SIZE, False, UTF8_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == UTF8_STRING && format_ret == 8) {
                const char *d = (const char *) data_ret;
                unsigned int s, n, index;
                for (s = 0, n = 0, index = 0; n < nitems_ret; n++) {
                    if (d[n] == '\0') {
                        delete [] p->desktop_names[index];
                        p->desktop_names[index++] = nstrndup((d + s), n - s + 1);
                        s = n + 1;
                    }
                }
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: desktop names array updated (%d entries)\n",
                    p->desktop_names.size());
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & ActiveWindow) {
        p->active = None;
        if (XGetWindowProperty(p->display, p->root, net_active_window, 0l, 1l,
                               False, XA_WINDOW, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32 && nitems_ret == 1) {
                p->active = *((Window *) data_ret);
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: active window = 0x%lx\n",
                    p->active);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WorkArea) {
        p->workarea.reset();
        if (XGetWindowProperty(p->display, p->root, net_workarea, 0l,
                               (p->number_of_desktops * 4), False, XA_CARDINAL,
                               &type_ret, &format_ret, &nitems_ret, &unused,
                               &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == (unsigned)(p->number_of_desktops * 4)) {
                long *d = (long *) data_ret;
                int i, j;
                for (i = 0, j = 0; i < p->number_of_desktops; i++) {
                    p->workarea[i].pos.x       = d[j++];
                    p->workarea[i].pos.y       = d[j++];
                    p->workarea[i].size.width  = d[j++];
                    p->workarea[i].size.height = d[j++];
                }
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: work area array updated (%d entries)\n",
                    p->workarea.size());
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & SupportingWMCheck) {
        p->supportwindow = None;
        delete[] p->name;
        p->name = NULL;
        if (XGetWindowProperty(p->display, p->root, net_supporting_wm_check,
                               0l, 1l, False, XA_WINDOW, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32 && nitems_ret == 1) {
                p->supportwindow = *((Window *) data_ret);
                unsigned char *name_ret;
                if (XGetWindowProperty(p->display, p->supportwindow,
                                       net_wm_name, 0l, MAX_PROP_SIZE, False,
                                       UTF8_STRING, &type_ret, &format_ret,
                                       &nitems_ret, &unused, &name_ret)
                        == Success) {
                    if (type_ret == UTF8_STRING && format_ret == 8)
                        p->name = nstrndup((const char *) name_ret, nitems_ret);
                    if (name_ret)
                        XFree(name_ret);
                }
            }
#ifdef    NETWMDEBUG
            fprintf(stderr,
                    "NETRootInfo::update: supporting window manager = '%s'\n",
                    p->name);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & VirtualRoots) {
        p->virtual_roots_count = 0;
        delete[] p->virtual_roots;
        p->virtual_roots = NULL;
        if (XGetWindowProperty(p->display, p->root, net_virtual_roots,
                               0, MAX_PROP_SIZE, False, XA_WINDOW, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32) {
                Window *wins = (Window *) data_ret;
                p->virtual_roots_count = nitems_ret;
                p->virtual_roots = nwindup(wins, p->virtual_roots_count);
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::updated: virtual roots updated (%ld windows)\n",
                    p->virtual_roots_count);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2ShowingDesktop) {
        p->showing_desktop = false;
        if (XGetWindowProperty(p->display, p->root, net_showing_desktop,
                               0, MAX_PROP_SIZE, False, XA_CARDINAL, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 && nitems_ret == 1) {
                p->showing_desktop = *((long *) data_ret);
            }
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETRootInfo::update: showing desktop = %d\n",
                    p->showing_desktop);
#endif
            if (data_ret)
                XFree(data_ret);
        }
    }
}


Display *NETRootInfo::x11Display() const
{
    return p->display;
}


Window NETRootInfo::rootWindow() const
{
    return p->root;
}


Window NETRootInfo::supportWindow() const
{
    return p->supportwindow;
}


const char *NETRootInfo::wmName() const
{
    return p->name;
}


int NETRootInfo::screenNumber() const
{
    return p->screen;
}


unsigned long NETRootInfo::supported() const
{
    return role == WindowManager
           ? p->properties[ PROTOCOLS ]
           : p->client_properties[ PROTOCOLS ];
}

const unsigned long* NETRootInfo::supportedProperties() const
{
    return p->properties;
}

const unsigned long* NETRootInfo::passedProperties() const
{
    return role == WindowManager
           ? p->properties
           : p->client_properties;
}

bool NETRootInfo::isSupported(NET::Property property) const
{
    return p->properties[ PROTOCOLS ] & property;
}

bool NETRootInfo::isSupported(NET::Property2 property) const
{
    return p->properties[ PROTOCOLS2 ] & property;
}

bool NETRootInfo::isSupported(NET::WindowType type) const
{
    return p->properties[ WINDOW_TYPES ] & type;
}

bool NETRootInfo::isSupported(NET::State state) const
{
    return p->properties[ STATES ] & state;
}

bool NETRootInfo::isSupported(NET::Action action) const
{
    return p->properties[ ACTIONS ] & action;
}

const Window *NETRootInfo::clientList() const
{
    return p->clients;
}


int NETRootInfo::clientListCount() const
{
    return p->clients_count;
}


const Window *NETRootInfo::clientListStacking() const
{
    return p->stacking;
}


int NETRootInfo::clientListStackingCount() const
{
    return p->stacking_count;
}


const Window *NETRootInfo::kdeSystemTrayWindows() const
{
    return p->kde_system_tray_windows;
}


int NETRootInfo::kdeSystemTrayWindowsCount() const
{
    return p->kde_system_tray_windows_count;
}


NETSize NETRootInfo::desktopGeometry(int) const
{
    return p->geometry.width != 0 ? p->geometry : p->rootSize;
}


NETPoint NETRootInfo::desktopViewport(int desktop) const
{
    if (desktop < 1) {
        NETPoint pt; // set to (0,0)
        return pt;
    }
    return p->viewport[desktop - 1];
}


NETRect NETRootInfo::workArea(int desktop) const
{
    if (desktop < 1) {
        NETRect rt;
        return rt;
    }
    return p->workarea[desktop - 1];
}


const char *NETRootInfo::desktopName(int desktop) const
{
    if (desktop < 1) {
        return 0;
    }
    return p->desktop_names[desktop - 1];
}


const Window *NETRootInfo::virtualRoots() const
{
    return p->virtual_roots;
}


int NETRootInfo::virtualRootsCount() const
{
    return p->virtual_roots_count;
}


int NETRootInfo::numberOfDesktops() const
{
    return p->number_of_desktops == 0 ? 1 : p->number_of_desktops;
}


int NETRootInfo::currentDesktop() const
{
    return p->current_desktop == 0 ? 1 : p->current_desktop;
}


Window NETRootInfo::activeWindow() const
{
    return p->active;
}


// NETWinInfo stuffs

const int NETWinInfo::OnAllDesktops = NET::OnAllDesktops;

NETWinInfo::NETWinInfo(Display *display, Window window, Window rootWindow,
                       const unsigned long properties[], int properties_size,
                       Role role)
{
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETWinInfo::NETWinInfo: constructing object with role '%s'\n",
            (role == WindowManager) ? "WindowManager" : "Client");
#endif
    p = new NETWinInfoPrivate;
    p->ref = 1;
    p->display = display;
    p->window = window;
    p->root = rootWindow;
    p->mapping_state = Withdrawn;
    p->mapping_state_dirty = True;
    p->state = 0;
    p->types[ 0 ] = Unknown;
    p->name = (char *) 0;
    p->visible_name = (char *) 0;
    p->icon_name = (char *) 0;
    p->visible_icon_name = (char *) 0;
    p->desktop = p->pid = p->handled_icons = 0;
    p->user_time = -1U;
    p->startup_id = NULL;
    p->transient_for = None;
    p->window_group = None;
    p->allowed_actions = 0;
    p->has_net_support = false;
    p->class_class = (char*) 0;
    p->class_name = (char*) 0;
    p->role = (char*) 0;
    p->client_machine = (char*) 0;
    // p->strut.left = p->strut.right = p->strut.top = p->strut.bottom = 0;
    // p->frame_strut.left = p->frame_strut.right = p->frame_strut.top =
    // p->frame_strut.bottom = 0;
    p->kde_system_tray_win_for = 0;
    for (int i = 0;
            i < PROPERTIES_SIZE;
            ++i)
        p->properties[ i ] = 0;
    if (properties_size > PROPERTIES_SIZE)
        properties_size = PROPERTIES_SIZE;
    for (int i = 0;
            i < properties_size;
            ++i)
        p->properties[ i ] = properties[ i ];
    p->icon_count = 0;
    this->role = role;
    if (! netwm_atoms_created) create_atoms(p->display);
    update(p->properties);
}


NETWinInfo::NETWinInfo(Display *display, Window window, Window rootWindow,
                       unsigned long properties, Role role)
{
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETWinInfo::NETWinInfo: constructing object with role '%s'\n",
            (role == WindowManager) ? "WindowManager" : "Client");
#endif
    p = new NETWinInfoPrivate;
    p->ref = 1;
    p->display = display;
    p->window = window;
    p->root = rootWindow;
    p->mapping_state = Withdrawn;
    p->mapping_state_dirty = True;
    p->state = 0;
    p->types[ 0 ] = Unknown;
    p->name = (char *) 0;
    p->visible_name = (char *) 0;
    p->icon_name = (char *) 0;
    p->visible_icon_name = (char *) 0;
    p->desktop = p->pid = p->handled_icons = 0;
    p->user_time = -1U;
    p->startup_id = NULL;
    p->transient_for = None;
    p->window_group = None;
    p->allowed_actions = 0;
    p->has_net_support = false;
    p->class_class = (char*) 0;
    p->class_name = (char*) 0;
    p->role = (char*) 0;
    p->client_machine = (char*) 0;
    // p->strut.left = p->strut.right = p->strut.top = p->strut.bottom = 0;
    // p->frame_strut.left = p->frame_strut.right = p->frame_strut.top =
    // p->frame_strut.bottom = 0;
    p->kde_system_tray_win_for = 0;
    for (int i = 0;
            i < PROPERTIES_SIZE;
            ++i)
        p->properties[ i ] = 0;
    p->properties[ PROTOCOLS ] = properties;
    p->icon_count = 0;
    this->role = role;
    if (! netwm_atoms_created) create_atoms(p->display);
    update(p->properties);
}


NETWinInfo::NETWinInfo(const NETWinInfo &wininfo)
{
    p = wininfo.p;
    p->ref++;
}


NETWinInfo::~NETWinInfo()
{
    refdec_nwi(p);
    if (! p->ref) delete p;
}


// assignment operator

const NETWinInfo &NETWinInfo::operator=(const NETWinInfo & wininfo)
{
#ifdef   NETWMDEBUG
    fprintf(stderr, "NETWinInfo::operator=()\n");
#endif
    if (p != wininfo.p) {
        refdec_nwi(p);
        if (! p->ref) delete p;
    }
    p = wininfo.p;
    role = wininfo.role;
    p->ref++;
    return *this;
}


void NETWinInfo::setIcon(NETIcon icon, Bool replace)
{
    setIconInternal(p->icons, p->icon_count, net_wm_icon, icon, replace);
}

void NETWinInfo::setIconInternal(NETRArray<NETIcon>& icons, int& icon_count, Atom property, NETIcon icon, Bool replace)
{
    if (role != Client) return;
    int proplen, i, sz, j;
    if (replace) {
        for (i = 0; i < icons.size(); i++) {
            delete [] icons[i].data;
            icons[i].data = 0;
            icons[i].size.width = 0;
            icons[i].size.height = 0;
        }
        icon_count = 0;
    }
    // assign icon
    icons[icon_count] = icon;
    icon_count++;
    // do a deep copy, we want to own the data
    NETIcon &ni = icons[icon_count - 1];
    sz = ni.size.width * ni.size.height;
    CARD32 *d = new CARD32[sz];
    ni.data = (unsigned char *) d;
    memcpy(d, icon.data, sz * sizeof(CARD32));
    // compute property length
    for (i = 0, proplen = 0; i < icon_count; i++) {
        proplen += 2 + (icons[i].size.width *
                        icons[i].size.height);
    }
    CARD32 *d32;
    long *prop = new long[proplen], *pprop = prop;
    for (i = 0; i < icon_count; i++) {
        // copy size into property
        *pprop++ = icons[i].size.width;
        *pprop++ = icons[i].size.height;
        // copy data into property
        sz = (icons[i].size.width * icons[i].size.height);
        d32 = (CARD32 *) icons[i].data;
        for (j = 0; j < sz; j++) *pprop++ = *d32++;
    }
    XChangeProperty(p->display, p->window, property, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) prop, proplen);
    delete [] prop;
}


void NETWinInfo::setIconGeometry(NETRect geometry)
{
    if (role != Client) return;
    p->icon_geom = geometry;
    if (geometry.size.width == 0)   // empty
        XDeleteProperty(p->display, p->window, net_wm_icon_geometry);
    else {
        long data[4];
        data[0] = geometry.pos.x;
        data[1] = geometry.pos.y;
        data[2] = geometry.size.width;
        data[3] = geometry.size.height;
        XChangeProperty(p->display, p->window, net_wm_icon_geometry, XA_CARDINAL,
                        32, PropModeReplace, (unsigned char *) data, 4);
    }
}


void NETWinInfo::setExtendedStrut(const NETExtendedStrut& extended_strut)
{
    if (role != Client) return;
    p->extended_strut = extended_strut;
    long data[12];
    data[0] = extended_strut.left_width;
    data[1] = extended_strut.right_width;
    data[2] = extended_strut.top_width;
    data[3] = extended_strut.bottom_width;
    data[4] = extended_strut.left_start;
    data[5] = extended_strut.left_end;
    data[6] = extended_strut.right_start;
    data[7] = extended_strut.right_end;
    data[8] = extended_strut.top_start;
    data[9] = extended_strut.top_end;
    data[10] = extended_strut.bottom_start;
    data[11] = extended_strut.bottom_end;
    XChangeProperty(p->display, p->window, net_wm_extended_strut, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) data, 12);
}


void NETWinInfo::setStrut(NETStrut strut)
{
    if (role != Client) return;
    p->strut = strut;
    long data[4];
    data[0] = strut.left;
    data[1] = strut.right;
    data[2] = strut.top;
    data[3] = strut.bottom;
    XChangeProperty(p->display, p->window, net_wm_strut, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) data, 4);
}


void NETWinInfo::setState(unsigned long state, unsigned long mask)
{
    if (p->mapping_state_dirty)
        updateWMState();
    // setState() needs to know the current state, so read it even if not requested
    if ((p->properties[ PROTOCOLS ] & WMState) == 0) {
        p->properties[ PROTOCOLS ] |= WMState;
        unsigned long props[ PROPERTIES_SIZE ] = { WMState, 0 };
        assert(PROPERTIES_SIZE == 2);   // add elements above
        update(props);
        p->properties[ PROTOCOLS ] &= ~WMState;
    }
    if (role == Client && p->mapping_state != Withdrawn) {
#ifdef NETWMDEBUG
        fprintf(stderr, "NETWinInfo::setState (0x%lx, 0x%lx) (Client)\n",
                state, mask);
#endif // NETWMDEBUG
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_wm_state;
        e.xclient.display = p->display;
        e.xclient.window = p->window;
        e.xclient.format = 32;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        if ((mask & Modal) && ((p->state & Modal) != (state & Modal))) {
            e.xclient.data.l[0] = (state & Modal) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_modal;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & Sticky) && ((p->state & Sticky) != (state & Sticky))) {
            e.xclient.data.l[0] = (state & Sticky) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_sticky;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & Max) && (((p->state&mask) & Max) != (state & Max))) {
            unsigned long wishstate = (p->state & ~mask) | (state & mask);
            if (((wishstate & MaxHoriz) != (p->state & MaxHoriz))
                    && ((wishstate & MaxVert) != (p->state & MaxVert))) {
                if ((wishstate & Max) == Max) {
                    e.xclient.data.l[0] = 1;
                    e.xclient.data.l[1] = net_wm_state_max_horiz;
                    e.xclient.data.l[2] = net_wm_state_max_vert;
                    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
                } else if ((wishstate & Max) == 0) {
                    e.xclient.data.l[0] = 0;
                    e.xclient.data.l[1] = net_wm_state_max_horiz;
                    e.xclient.data.l[2] = net_wm_state_max_vert;
                    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
                } else {
                    e.xclient.data.l[0] = (wishstate & MaxHoriz) ? 1 : 0;
                    e.xclient.data.l[1] = net_wm_state_max_horiz;
                    e.xclient.data.l[2] = 0;
                    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
                    e.xclient.data.l[0] = (wishstate & MaxVert) ? 1 : 0;
                    e.xclient.data.l[1] = net_wm_state_max_vert;
                    e.xclient.data.l[2] = 0;
                    XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
                }
            } else if ((wishstate & MaxVert) != (p->state & MaxVert)) {
                e.xclient.data.l[0] = (wishstate & MaxVert) ? 1 : 0;
                e.xclient.data.l[1] = net_wm_state_max_vert;
                e.xclient.data.l[2] = 0;
                XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
            } else if ((wishstate & MaxHoriz) != (p->state & MaxHoriz)) {
                e.xclient.data.l[0] = (wishstate & MaxHoriz) ? 1 : 0;
                e.xclient.data.l[1] = net_wm_state_max_horiz;
                e.xclient.data.l[2] = 0;
                XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
            }
        }
        if ((mask & Shaded) && ((p->state & Shaded) != (state & Shaded))) {
            e.xclient.data.l[0] = (state & Shaded) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_shaded;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & SkipTaskbar) &&
                ((p->state & SkipTaskbar) != (state & SkipTaskbar))) {
            e.xclient.data.l[0] = (state & SkipTaskbar) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_skip_taskbar;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & SkipPager) &&
                ((p->state & SkipPager) != (state & SkipPager))) {
            e.xclient.data.l[0] = (state & SkipPager) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_skip_pager;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & Hidden) &&
                ((p->state & Hidden) != (state & Hidden))) {
            e.xclient.data.l[0] = (state & Hidden) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_hidden;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & FullScreen) &&
                ((p->state & FullScreen) != (state & FullScreen))) {
            e.xclient.data.l[0] = (state & FullScreen) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_fullscreen;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & KeepAbove) &&
                ((p->state & KeepAbove) != (state & KeepAbove))) {
            e.xclient.data.l[0] = (state & KeepAbove) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_above;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & KeepBelow) &&
                ((p->state & KeepBelow) != (state & KeepBelow))) {
            e.xclient.data.l[0] = (state & KeepBelow) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_below;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & StaysOnTop) && ((p->state & StaysOnTop) != (state & StaysOnTop))) {
            e.xclient.data.l[0] = (state & StaysOnTop) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_stays_on_top;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
        if ((mask & DemandsAttention) &&
                ((p->state & DemandsAttention) != (state & DemandsAttention))) {
            e.xclient.data.l[0] = (state & DemandsAttention) ? 1 : 0;
            e.xclient.data.l[1] = net_wm_state_demands_attention;
            e.xclient.data.l[2] = 0l;
            XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
        }
    } else {
        p->state &= ~mask;
        p->state |= state;
        long data[50];
        int count = 0;
        // hints
        if (p->state & Modal) data[count++] = net_wm_state_modal;
        if (p->state & MaxVert) data[count++] = net_wm_state_max_vert;
        if (p->state & MaxHoriz) data[count++] = net_wm_state_max_horiz;
        if (p->state & Shaded) data[count++] = net_wm_state_shaded;
        if (p->state & Hidden) data[count++] = net_wm_state_hidden;
        if (p->state & FullScreen) data[count++] = net_wm_state_fullscreen;
        if (p->state & DemandsAttention) data[count++] = net_wm_state_demands_attention;
        // policy
        if (p->state & KeepAbove) data[count++] = net_wm_state_above;
        if (p->state & KeepBelow) data[count++] = net_wm_state_below;
        if (p->state & StaysOnTop) data[count++] = net_wm_state_stays_on_top;
        if (p->state & Sticky) data[count++] = net_wm_state_sticky;
        if (p->state & SkipTaskbar) data[count++] = net_wm_state_skip_taskbar;
        if (p->state & SkipPager) data[count++] = net_wm_state_skip_pager;
#ifdef NETWMDEBUG
        fprintf(stderr, "NETWinInfo::setState: setting state property (%d)\n", count);
        for (int i = 0; i < count; i++) {
            char* data_ret = XGetAtomName(p->display, (Atom) data[i]);
            fprintf(stderr, "NETWinInfo::setState:   state %ld '%s'\n",
                    data[i], data_ret);
            if (data_ret)
                XFree(data_ret);
        }
#endif
        XChangeProperty(p->display, p->window, net_wm_state, XA_ATOM, 32,
                        PropModeReplace, (unsigned char *) data, count);
    }
}


void NETWinInfo::setWindowType(WindowType type)
{
    if (role != Client) return;
    int len;
    long data[2];
    switch (type) {
    case Override:
        // spec extension: override window type.  we must comply with the spec
        // and provide a fall back (normal seems best)
        data[0] = kde_net_wm_window_type_override;
        data[1] = net_wm_window_type_normal;
        len = 2;
        break;
    case  Dialog:
        data[0] = net_wm_window_type_dialog;
        data[1] = None;
        len = 1;
        break;
    case Menu:
        data[0] = net_wm_window_type_menu;
        data[1] = None;
        len = 1;
        break;
    case TopMenu:
        // spec extension: override window type.  we must comply with the spec
        // and provide a fall back (dock seems best)
        data[0] = kde_net_wm_window_type_topmenu;
        data[1] = net_wm_window_type_dock;
        len = 2;
        break;
    case Tool:
        data[0] = net_wm_window_type_toolbar;
        data[1] = None;
        len = 1;
        break;
    case Dock:
        data[0] = net_wm_window_type_dock;
        data[1] = None;
        len = 1;
        break;
    case Desktop:
        data[0] = net_wm_window_type_desktop;
        data[1] = None;
        len = 1;
        break;
    case Utility:
        data[0] = net_wm_window_type_utility;
        data[1] = net_wm_window_type_dialog; // fallback for old netwm version
        len = 2;
        break;
    case Splash:
        data[0] = net_wm_window_type_splash;
        data[1] = net_wm_window_type_dock; // fallback (dock seems best)
        len = 2;
        break;
    default:
    case Normal:
        data[0] = net_wm_window_type_normal;
        data[1] = None;
        len = 1;
        break;
    }
    XChangeProperty(p->display, p->window, net_wm_window_type, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *) &data, len);
}


void NETWinInfo::setName(const char *name)
{
    if (role != Client) return;
    delete [] p->name;
    p->name = nstrdup(name);
    if (p->name[ 0 ] != '\0')
        XChangeProperty(p->display, p->window, net_wm_name, UTF8_STRING, 8,
                        PropModeReplace, (unsigned char *) p->name,
                        strlen(p->name));
    else
        XDeleteProperty(p->display, p->window, net_wm_name);
}


void NETWinInfo::setVisibleName(const char *visibleName)
{
    if (role != WindowManager) return;
    delete [] p->visible_name;
    p->visible_name = nstrdup(visibleName);
    if (p->visible_name[ 0 ] != '\0')
        XChangeProperty(p->display, p->window, net_wm_visible_name, UTF8_STRING, 8,
                        PropModeReplace, (unsigned char *) p->visible_name,
                        strlen(p->visible_name));
    else
        XDeleteProperty(p->display, p->window, net_wm_visible_name);
}


void NETWinInfo::setIconName(const char *iconName)
{
    if (role != Client) return;
    delete [] p->icon_name;
    p->icon_name = nstrdup(iconName);
    if (p->icon_name[ 0 ] != '\0')
        XChangeProperty(p->display, p->window, net_wm_icon_name, UTF8_STRING, 8,
                        PropModeReplace, (unsigned char *) p->icon_name,
                        strlen(p->icon_name));
    else
        XDeleteProperty(p->display, p->window, net_wm_icon_name);
}


void NETWinInfo::setVisibleIconName(const char *visibleIconName)
{
    if (role != WindowManager) return;
    delete [] p->visible_icon_name;
    p->visible_icon_name = nstrdup(visibleIconName);
    if (p->visible_icon_name[ 0 ] != '\0')
        XChangeProperty(p->display, p->window, net_wm_visible_icon_name, UTF8_STRING, 8,
                        PropModeReplace, (unsigned char *) p->visible_icon_name,
                        strlen(p->visible_icon_name));
    else
        XDeleteProperty(p->display, p->window, net_wm_visible_icon_name);
}


void NETWinInfo::setDesktop(int desktop)
{
    if (p->mapping_state_dirty)
        updateWMState();
    if (role == Client && p->mapping_state != Withdrawn) {
        // we only send a ClientMessage if we are 1) a client and 2) managed
        if (desktop == 0)
            return; // we can't do that while being managed
        XEvent e;
        e.xclient.type = ClientMessage;
        e.xclient.message_type = net_wm_desktop;
        e.xclient.display = p->display;
        e.xclient.window = p->window;
        e.xclient.format = 32;
        e.xclient.data.l[0] = desktop == OnAllDesktops ? OnAllDesktops : desktop - 1;
        e.xclient.data.l[1] = 0l;
        e.xclient.data.l[2] = 0l;
        e.xclient.data.l[3] = 0l;
        e.xclient.data.l[4] = 0l;
        XSendEvent(p->display, p->root, False, netwm_sendevent_mask, &e);
    } else {
        // otherwise we just set or remove the property directly
        p->desktop = desktop;
        long d = desktop;
        if (d != OnAllDesktops) {
            if (d == 0) {
                XDeleteProperty(p->display, p->window, net_wm_desktop);
                return;
            }
            d -= 1;
        }
        XChangeProperty(p->display, p->window, net_wm_desktop, XA_CARDINAL, 32,
                        PropModeReplace, (unsigned char *) &d, 1);
    }
}


void NETWinInfo::setPid(int pid)
{
    if (role != Client) return;
    p->pid = pid;
    long d = pid;
    XChangeProperty(p->display, p->window, net_wm_pid, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) &d, 1);
}


void NETWinInfo::setHandledIcons(Bool handled)
{
    if (role != Client) return;
    p->handled_icons = handled;
    long d = handled;
    XChangeProperty(p->display, p->window, net_wm_handled_icons, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) &d, 1);
}

void NETWinInfo::setStartupId(const char* id)
{
    if (role != Client) return;
    delete[] p->startup_id;
    p->startup_id = nstrdup(id);
    XChangeProperty(p->display, p->window, net_startup_id, UTF8_STRING, 8,
                    PropModeReplace, reinterpret_cast< unsigned char* >(p->startup_id),
                    strlen(p->startup_id));
}

void NETWinInfo::setAllowedActions(unsigned long actions)
{
    if (role != WindowManager)
        return;
    long data[50];
    int count = 0;
    p->allowed_actions = actions;
    if (p->allowed_actions & ActionMove) data[count++] = net_wm_action_move;
    if (p->allowed_actions & ActionResize) data[count++] = net_wm_action_resize;
    if (p->allowed_actions & ActionMinimize) data[count++] = net_wm_action_minimize;
    if (p->allowed_actions & ActionShade) data[count++] = net_wm_action_shade;
    if (p->allowed_actions & ActionStick) data[count++] = net_wm_action_stick;
    if (p->allowed_actions & ActionMaxVert) data[count++] = net_wm_action_max_vert;
    if (p->allowed_actions & ActionMaxHoriz) data[count++] = net_wm_action_max_horiz;
    if (p->allowed_actions & ActionFullScreen) data[count++] = net_wm_action_fullscreen;
    if (p->allowed_actions & ActionChangeDesktop) data[count++] = net_wm_action_change_desk;
    if (p->allowed_actions & ActionClose) data[count++] = net_wm_action_close;
#ifdef NETWMDEBUG
    fprintf(stderr, "NETWinInfo::setAllowedActions: setting property (%d)\n", count);
    for (int i = 0; i < count; i++) {
        char* data_ret = XGetAtomName(p->display, (Atom) data[i]);
        fprintf(stderr, "NETWinInfo::setAllowedActions:   action %ld '%s'\n",
                data[i], data_ret);
        if (data_ret)
            XFree(data_ret);
    }
#endif
    XChangeProperty(p->display, p->window, net_wm_allowed_actions, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *) data, count);
}

void NETWinInfo::setKDESystemTrayWinFor(Window window)
{
    if (role != Client) return;
    p->kde_system_tray_win_for = window;
    XChangeProperty(p->display, p->window, kde_net_wm_system_tray_window_for,
                    XA_WINDOW, 32, PropModeReplace,
                    (unsigned char *) &(p->kde_system_tray_win_for), 1);
}


void NETWinInfo::setKDEFrameStrut(NETStrut strut)
{
    setFrameExtents(strut);
}

void NETWinInfo::setFrameExtents(NETStrut strut)
{
    if (role != WindowManager) return;
    p->frame_strut = strut;
    long d[4];
    d[0] = strut.left;
    d[1] = strut.right;
    d[2] = strut.top;
    d[3] = strut.bottom;
    XChangeProperty(p->display, p->window, net_frame_extents, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) d, 4);
    XChangeProperty(p->display, p->window, kde_net_wm_frame_strut, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) d, 4);
}


void NETWinInfo::kdeGeometry(NETRect& frame, NETRect& window)
{
    if (p->win_geom.size.width == 0 || p->win_geom.size.height == 0) {
        Window unused;
        int x, y;
        unsigned int w, h, junk;
        XGetGeometry(p->display, p->window, &unused, &x, &y, &w, &h, &junk, &junk);
        XTranslateCoordinates(p->display, p->window, p->root, 0, 0, &x, &y, &unused
                             );
        p->win_geom.pos.x = x;
        p->win_geom.pos.y = y;
        p->win_geom.size.width = w;
        p->win_geom.size.height = h;
    }
// TODO try to work also without _KDE_NET_WM_FRAME_STRUT
    window = p->win_geom;
    frame.pos.x = window.pos.x - p->frame_strut.left;
    frame.pos.y = window.pos.y - p->frame_strut.top;
    frame.size.width = window.size.width + p->frame_strut.left + p->frame_strut.right;
    frame.size.height = window.size.height + p->frame_strut.top + p->frame_strut.bottom;
}


NETIcon NETWinInfo::icon(int width, int height) const
{
    return iconInternal(p->icons, p->icon_count, width, height);
}

NETIcon NETWinInfo::iconInternal(NETRArray<NETIcon>& icons, int icon_count, int width, int height) const
{
    NETIcon result;
    if (!icon_count) {
        result.size.width = 0;
        result.size.height = 0;
        result.data = 0;
        return result;
    }
    result = icons[0];
    // find the icon that's closest in size to w x h...
    // return the first icon if w and h are -1
    if (width == height && height == -1) return result;
    int i;
    for (i = 0; i < icons.size(); i++) {
        if ((icons[i].size.width >= width &&
                icons[i].size.width < result.size.width) &&
                (icons[i].size.height >= height &&
                 icons[i].size.height < result.size.height))
            result = icons[i];
    }
    return result;
}

void NETWinInfo::setUserTime(Time time)
{
    if (role != Client) return;
    p->user_time = time;
    long d = time;
    XChangeProperty(p->display, p->window, net_wm_user_time, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char *) &d, 1);
}


unsigned long NETWinInfo::event(XEvent *ev)
{
    unsigned long props[ 1 ];
    event(ev, props, 1);
    return props[ 0 ];
}

void NETWinInfo::event(XEvent *event, unsigned long* properties, int properties_size)
{
    unsigned long props[ PROPERTIES_SIZE ] = { 0, 0 };
    assert(PROPERTIES_SIZE == 2);   // add elements above
    unsigned long& dirty = props[ PROTOCOLS ];
    unsigned long& dirty2 = props[ PROTOCOLS2 ];
    bool do_update = false;
    if (role == WindowManager && event->type == ClientMessage &&
            event->xclient.format == 32) {
#ifdef NETWMDEBUG
        fprintf(stderr, "NETWinInfo::event: handling ClientMessage event\n");
#endif // NETWMDEBUG
        if (event->xclient.message_type == net_wm_state) {
            dirty = WMState;
            // we need to generate a change mask
#ifdef NETWMDEBUG
            fprintf(stderr,
                    "NETWinInfo::event: state client message, getting new state/mask\n");
#endif
            int i;
            long state = 0, mask = 0;
            for (i = 1; i < 3; i++) {
#ifdef NETWMDEBUG
                char* debug_txt = XGetAtomName(p->display, (Atom) event->xclient.data.l[i]);
                fprintf(stderr, "NETWinInfo::event:  message %ld '%s'\n",
                        event->xclient.data.l[i], debug_txt);
                if (debug_txt)
                    XFree(debug_txt);
#endif
                if ((Atom) event->xclient.data.l[i] == net_wm_state_modal)
                    mask |= Modal;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_sticky)
                    mask |= Sticky;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_max_vert)
                    mask |= MaxVert;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_max_horiz)
                    mask |= MaxHoriz;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_shaded)
                    mask |= Shaded;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_skip_taskbar)
                    mask |= SkipTaskbar;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_skip_pager)
                    mask |= SkipPager;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_hidden)
                    mask |= Hidden;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_fullscreen)
                    mask |= FullScreen;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_above)
                    mask |= KeepAbove;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_below)
                    mask |= KeepBelow;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_demands_attention)
                    mask |= DemandsAttention;
                else if ((Atom) event->xclient.data.l[i] == net_wm_state_stays_on_top)
                    mask |= StaysOnTop;
            }
            // when removing, we just leave newstate == 0
            switch (event->xclient.data.l[0]) {
            case 1: // set
                // to set... the change state should be the same as the mask
                state = mask;
                break;
            case 2: // toggle
                // to toggle, we need to xor the current state with the new state
                state = (p->state & mask) ^ mask;
                break;
            default:
                // to clear state, the new state should stay zero
                ;
            }
#ifdef NETWMDEBUG
            fprintf(stderr, "NETWinInfo::event: calling changeState(%lx, %lx)\n",
                    state, mask);
#endif
            changeState(state, mask);
        } else if (event->xclient.message_type == net_wm_desktop) {
            dirty = WMDesktop;
            if (event->xclient.data.l[0] == OnAllDesktops)
                changeDesktop(OnAllDesktops);
            else
                changeDesktop(event->xclient.data.l[0] + 1);
        }
    }
    if (event->type == PropertyNotify) {
#ifdef    NETWMDEBUG
        fprintf(stderr, "NETWinInfo::event: handling PropertyNotify event\n");
#endif
        XEvent pe = *event;
        Bool done = False;
        Bool compaction = False;
        while (! done) {
#ifdef    NETWMDEBUG
            fprintf(stderr, "NETWinInfo::event: loop fire\n");
#endif
            if (pe.xproperty.atom == net_wm_name)
                dirty |= WMName;
            else if (pe.xproperty.atom == net_wm_visible_name)
                dirty |= WMVisibleName;
            else if (pe.xproperty.atom == net_wm_desktop)
                dirty |= WMDesktop;
            else if (pe.xproperty.atom == net_wm_window_type)
                dirty |= WMWindowType;
            else if (pe.xproperty.atom == net_wm_state)
                dirty |= WMState;
            else if (pe.xproperty.atom == net_wm_strut)
                dirty |= WMStrut;
            else if (pe.xproperty.atom == net_wm_extended_strut)
                dirty2 |= WM2ExtendedStrut;
            else if (pe.xproperty.atom == net_wm_icon_geometry)
                dirty |= WMIconGeometry;
            else if (pe.xproperty.atom == net_wm_icon)
                dirty |= WMIcon;
            else if (pe.xproperty.atom == net_wm_pid)
                dirty |= WMPid;
            else if (pe.xproperty.atom == net_wm_handled_icons)
                dirty |= WMHandledIcons;
            else if (pe.xproperty.atom == net_startup_id)
                dirty2 |= WM2StartupId;
            else if (pe.xproperty.atom == net_wm_allowed_actions)
                dirty2 |= WM2AllowedActions;
            else if (pe.xproperty.atom == kde_net_wm_system_tray_window_for)
                dirty |= WMKDESystemTrayWinFor;
            else if (pe.xproperty.atom == xa_wm_state)
                dirty |= XAWMState;
            else if (pe.xproperty.atom == net_frame_extents)
                dirty |= WMFrameExtents;
            else if (pe.xproperty.atom == kde_net_wm_frame_strut)
                dirty |= WMKDEFrameStrut;
            else if (pe.xproperty.atom == net_wm_icon_name)
                dirty |= WMIconName;
            else if (pe.xproperty.atom == net_wm_visible_icon_name)
                dirty |= WMVisibleIconName;
            else if (pe.xproperty.atom == net_wm_user_time)
                dirty2 |= WM2UserTime;
            else if (pe.xproperty.atom == XA_WM_HINTS)
                dirty2 |= WM2GroupLeader;
            else if (pe.xproperty.atom == XA_WM_TRANSIENT_FOR)
                dirty2 |= WM2TransientFor;
            else if (pe.xproperty.atom == XA_WM_CLASS)
                dirty2 |= WM2WindowClass;
            else if (pe.xproperty.atom == wm_window_role)
                dirty2 |= WM2WindowRole;
            else if (pe.xproperty.atom == XA_WM_CLIENT_MACHINE)
                dirty2 |= WM2ClientMachine;
            else {
#ifdef    NETWMDEBUG
                fprintf(stderr, "NETWinInfo::event: putting back event and breaking\n");
#endif
                if (compaction)
                    XPutBackEvent(p->display, &pe);
                break;
            }
            if (XCheckTypedWindowEvent(p->display, p->window, PropertyNotify, &pe))
                compaction = True;
            else
                break;
        }
        do_update = true;
    } else if (event->type == ConfigureNotify) {
#ifdef NETWMDEBUG
        fprintf(stderr, "NETWinInfo::event: handling ConfigureNotify event\n");
#endif
        dirty |= WMGeometry;
        // update window geometry
        p->win_geom.pos.x = event->xconfigure.x;
        p->win_geom.pos.y = event->xconfigure.y;
        p->win_geom.size.width = event->xconfigure.width;
        p->win_geom.size.height = event->xconfigure.height;
    }
    if (do_update)
        update(props);
    if (properties_size > PROPERTIES_SIZE)
        properties_size = PROPERTIES_SIZE;
    for (int i = 0;
            i < properties_size;
            ++i)
        properties[ i ] = props[ i ];
}

void NETWinInfo::updateWMState()
{
    unsigned long props[ PROPERTIES_SIZE ] = { XAWMState, 0 };
    assert(PROPERTIES_SIZE == 2);   // add elements above
    update(props);
}

void NETWinInfo::update(const unsigned long dirty_props[])
{
    Atom type_ret;
    int format_ret;
    unsigned long nitems_ret, unused;
    unsigned char *data_ret;
    unsigned long props[ PROPERTIES_SIZE ];
    for (int i = 0;
            i < PROPERTIES_SIZE;
            ++i)
        props[ i ] = dirty_props[ i ] & p->properties[ i ];
    const unsigned long& dirty = props[ PROTOCOLS ];
    const unsigned long& dirty2 = props[ PROTOCOLS2 ];
    // we *always* want to update WM_STATE if set in dirty_props
    if (dirty_props[ PROTOCOLS ] & XAWMState)
        props[ PROTOCOLS ] |= XAWMState;
    if (dirty & XAWMState) {
        p->mapping_state = Withdrawn;
        if (XGetWindowProperty(p->display, p->window, xa_wm_state, 0l, 1l,
                               False, xa_wm_state, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == xa_wm_state && format_ret == 32 &&
                    nitems_ret == 1) {
                long *state = (long *) data_ret;
                switch (*state) {
                case IconicState:
                    p->mapping_state = Iconic;
                    break;
                case NormalState:
                    p->mapping_state = Visible;
                    break;
                case WithdrawnState:
                default:
                    p->mapping_state = Withdrawn;
                    break;
                }
                p->mapping_state_dirty = False;
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMState) {
        p->state = 0;
        if (XGetWindowProperty(p->display, p->window, net_wm_state, 0l, 2048l,
                               False, XA_ATOM, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_ATOM && format_ret == 32 && nitems_ret > 0) {
                // determine window state
#ifdef NETWMDEBUG
                fprintf(stderr, "NETWinInfo::update: updating window state (%ld)\n",
                        nitems_ret);
#endif
                long *states = (long *) data_ret;
                unsigned long count;
                for (count = 0; count < nitems_ret; count++) {
#ifdef NETWMDEBUG
                    char* data_ret = XGetAtomName(p->display, (Atom) states[count]);
                    fprintf(stderr,
                            "NETWinInfo::update:   adding window state %ld '%s'\n",
                            states[count], data_ret);
                    if (data_ret)
                        XFree(data_ret);
#endif
                    if ((Atom) states[count] == net_wm_state_modal)
                        p->state |= Modal;
                    else if ((Atom) states[count] == net_wm_state_sticky)
                        p->state |= Sticky;
                    else if ((Atom) states[count] == net_wm_state_max_vert)
                        p->state |= MaxVert;
                    else if ((Atom) states[count] == net_wm_state_max_horiz)
                        p->state |= MaxHoriz;
                    else if ((Atom) states[count] == net_wm_state_shaded)
                        p->state |= Shaded;
                    else if ((Atom) states[count] == net_wm_state_skip_taskbar)
                        p->state |= SkipTaskbar;
                    else if ((Atom) states[count] == net_wm_state_skip_pager)
                        p->state |= SkipPager;
                    else if ((Atom) states[count] == net_wm_state_hidden)
                        p->state |= Hidden;
                    else if ((Atom) states[count] == net_wm_state_fullscreen)
                        p->state |= FullScreen;
                    else if ((Atom) states[count] == net_wm_state_above)
                        p->state |= KeepAbove;
                    else if ((Atom) states[count] == net_wm_state_below)
                        p->state |= KeepBelow;
                    else if ((Atom) states[count] == net_wm_state_demands_attention)
                        p->state |= DemandsAttention;
                    else if ((Atom) states[count] == net_wm_state_stays_on_top)
                        p->state |= StaysOnTop;
                }
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMDesktop) {
        p->desktop = 0;
        if (XGetWindowProperty(p->display, p->window, net_wm_desktop, 0l, 1l,
                               False, XA_CARDINAL, &type_ret,
                               &format_ret, &nitems_ret,
                               &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == 1) {
                p->desktop = *((long *) data_ret);
                if ((signed) p->desktop != OnAllDesktops)
                    p->desktop++;
                if (p->desktop == 0)
                    p->desktop = OnAllDesktops;
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMName) {
        delete[] p->name;
        p->name = NULL;
        if (XGetWindowProperty(p->display, p->window, net_wm_name, 0l,
                               MAX_PROP_SIZE, False, UTF8_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == UTF8_STRING && format_ret == 8 && nitems_ret > 0) {
                p->name = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMVisibleName) {
        delete[] p->visible_name;
        p->visible_name = NULL;
        if (XGetWindowProperty(p->display, p->window, net_wm_visible_name, 0l,
                               MAX_PROP_SIZE, False, UTF8_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == UTF8_STRING && format_ret == 8 && nitems_ret > 0) {
                p->visible_name = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMIconName) {
        delete[] p->icon_name;
        p->icon_name = NULL;
        if (XGetWindowProperty(p->display, p->window, net_wm_icon_name, 0l,
                               MAX_PROP_SIZE, False, UTF8_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == UTF8_STRING && format_ret == 8 && nitems_ret > 0) {
                p->icon_name = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMVisibleIconName) {
        delete[] p->visible_icon_name;
        p->visible_icon_name = NULL;
        if (XGetWindowProperty(p->display, p->window, net_wm_visible_icon_name, 0l,
                               MAX_PROP_SIZE, False, UTF8_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == UTF8_STRING && format_ret == 8 && nitems_ret > 0) {
                p->visible_icon_name = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMWindowType) {
        p->types.reset();
        p->types[ 0 ] = Unknown;
        p->has_net_support = false;
        if (XGetWindowProperty(p->display, p->window, net_wm_window_type, 0l, 2048l,
                               False, XA_ATOM, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_ATOM && format_ret == 32 && nitems_ret > 0) {
                // determine the window type
#ifdef NETWMDEBUG
                fprintf(stderr, "NETWinInfo::update: getting window type (%ld)\n",
                        nitems_ret);
#endif
                p->has_net_support = true;
                unsigned long count = 0;
                long *types = (long *) data_ret;
                int pos = 0;
                while (count < nitems_ret) {
                    // remember all window types we know
#ifdef NETWMDEBUG
                    char* debug_type = XGetAtomName(p->display, (Atom) types[count]);
                    fprintf(stderr,
                            "NETWinInfo::update:   examining window type %ld %s\n",
                            types[count], debug_type);
                    if (debug_type)
                        XFree(debug_type);
#endif
                    if ((Atom) types[count] == net_wm_window_type_normal)
                        p->types[ pos++ ] = Normal;
                    else if ((Atom) types[count] == net_wm_window_type_desktop)
                        p->types[ pos++ ] = Desktop;
                    else if ((Atom) types[count] == net_wm_window_type_dock)
                        p->types[ pos++ ] = Dock;
                    else if ((Atom) types[count] == net_wm_window_type_toolbar)
                        p->types[ pos++ ] = Tool;
                    else if ((Atom) types[count] == net_wm_window_type_menu)
                        p->types[ pos++ ] = Menu;
                    else if ((Atom) types[count] == net_wm_window_type_dialog)
                        p->types[ pos++ ] = Dialog;
                    else if ((Atom) types[count] == net_wm_window_type_utility)
                        p->types[ pos++ ] = Utility;
                    else if ((Atom) types[count] == net_wm_window_type_splash)
                        p->types[ pos++ ] = Splash;
                    else if ((Atom) types[count] == kde_net_wm_window_type_override)
                        p->types[ pos++ ] = Override;
                    else if ((Atom) types[count] == kde_net_wm_window_type_topmenu)
                        p->types[ pos++ ] = TopMenu;
                    count++;
                }
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMStrut) {
        p->strut = NETStrut();
        if (XGetWindowProperty(p->display, p->window, net_wm_strut, 0l, 4l,
                               False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == 4) {
                long *d = (long *) data_ret;
                p->strut.left   = d[0];
                p->strut.right  = d[1];
                p->strut.top    = d[2];
                p->strut.bottom = d[3];
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2ExtendedStrut) {
        p->extended_strut = NETExtendedStrut();
        if (XGetWindowProperty(p->display, p->window, net_wm_extended_strut, 0l, 12l,
                               False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == 12) {
                long *d = (long *) data_ret;
                p->extended_strut.left_width = d[0];
                p->extended_strut.right_width = d[1];
                p->extended_strut.top_width = d[2];
                p->extended_strut.bottom_width = d[3];
                p->extended_strut.left_start = d[4];
                p->extended_strut.left_end = d[5];
                p->extended_strut.right_start = d[6];
                p->extended_strut.right_end = d[7];
                p->extended_strut.top_start = d[8];
                p->extended_strut.top_end = d[9];
                p->extended_strut.bottom_start = d[10];
                p->extended_strut.bottom_end = d[11];
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMIconGeometry) {
        p->icon_geom = NETRect();
        if (XGetWindowProperty(p->display, p->window, net_wm_icon_geometry, 0l, 4l,
                               False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 &&
                    nitems_ret == 4) {
                long *d = (long *) data_ret;
                p->icon_geom.pos.x       = d[0];
                p->icon_geom.pos.y       = d[1];
                p->icon_geom.size.width  = d[2];
                p->icon_geom.size.height = d[3];
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMIcon) {
        readIcon(p->display, p->window, net_wm_icon, p->icons, p->icon_count);
    }
    if (dirty & WMKDESystemTrayWinFor) {
        p->kde_system_tray_win_for = 0;
        if (XGetWindowProperty(p->display, p->window, kde_net_wm_system_tray_window_for,
                               0l, 1l, False, XA_WINDOW, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_WINDOW && format_ret == 32 &&
                    nitems_ret == 1) {
                p->kde_system_tray_win_for = *((Window *) data_ret);
                if (p->kde_system_tray_win_for == 0)
                    p->kde_system_tray_win_for = p->root;
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMFrameExtents) {
        p->frame_strut = NETStrut();
        bool ok = false;
        if (XGetWindowProperty(p->display, p->window, net_frame_extents,
                               0l, 4l, False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret) == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 && nitems_ret == 4) {
                ok = true;
                long *d = (long *) data_ret;
                p->frame_strut.left   = d[0];
                p->frame_strut.right  = d[1];
                p->frame_strut.top    = d[2];
                p->frame_strut.bottom = d[3];
            }
            if (data_ret)
                XFree(data_ret);
        }
        if (!ok && XGetWindowProperty(p->display, p->window, kde_net_wm_frame_strut,
                                      0l, 4l, False, XA_CARDINAL, &type_ret, &format_ret,
                                      &nitems_ret, &unused, &data_ret) == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 && nitems_ret == 4) {
                ok = true;
                long *d = (long *) data_ret;
                p->frame_strut.left   = d[0];
                p->frame_strut.right  = d[1];
                p->frame_strut.top    = d[2];
                p->frame_strut.bottom = d[3];
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty & WMPid) {
        p->pid = 0;
        if (XGetWindowProperty(p->display, p->window, net_wm_pid, 0l, 1l,
                               False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret) == Success) {
            if (type_ret == XA_CARDINAL && format_ret == 32 && nitems_ret == 1) {
                p->pid = *((long *) data_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2StartupId) {
        delete[] p->startup_id;
        p->startup_id = NULL;
        if (XGetWindowProperty(p->display, p->window, net_startup_id, 0l,
                               MAX_PROP_SIZE, False, UTF8_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == UTF8_STRING && format_ret == 8 && nitems_ret > 0) {
                p->startup_id = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2AllowedActions) {
        p->allowed_actions = 0;
        if (XGetWindowProperty(p->display, p->window, net_wm_allowed_actions, 0l, 2048l,
                               False, XA_ATOM, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_ATOM && format_ret == 32 && nitems_ret > 0) {
                // determine actions
#ifdef NETWMDEBUG
                fprintf(stderr, "NETWinInfo::update: updating allowed actions (%ld)\n",
                        nitems_ret);
#endif
                long *actions = (long *) data_ret;
                unsigned long count;
                for (count = 0; count < nitems_ret; count++) {
#ifdef NETWMDEBUG
                    fprintf(stderr,
                            "NETWinInfo::update:   adding allowed action %ld '%s'\n",
                            actions[count],
                            XGetAtomName(p->display, (Atom) actions[count]));
#endif
                    if ((Atom) actions[count] == net_wm_action_move)
                        p->allowed_actions |= ActionMove;
                    if ((Atom) actions[count] == net_wm_action_resize)
                        p->allowed_actions |= ActionResize;
                    if ((Atom) actions[count] == net_wm_action_minimize)
                        p->allowed_actions |= ActionMinimize;
                    if ((Atom) actions[count] == net_wm_action_shade)
                        p->allowed_actions |= ActionShade;
                    if ((Atom) actions[count] == net_wm_action_stick)
                        p->allowed_actions |= ActionStick;
                    if ((Atom) actions[count] == net_wm_action_max_vert)
                        p->allowed_actions |= ActionMaxVert;
                    if ((Atom) actions[count] == net_wm_action_max_horiz)
                        p->allowed_actions |= ActionMaxHoriz;
                    if ((Atom) actions[count] == net_wm_action_fullscreen)
                        p->allowed_actions |= ActionFullScreen;
                    if ((Atom) actions[count] == net_wm_action_change_desk)
                        p->allowed_actions |= ActionChangeDesktop;
                    if ((Atom) actions[count] == net_wm_action_close)
                        p->allowed_actions |= ActionClose;
                }
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2UserTime) {
        p->user_time = -1U;
        if (XGetWindowProperty(p->display, p->window, net_wm_user_time, 0l, 1l,
                               False, XA_CARDINAL, &type_ret, &format_ret,
                               &nitems_ret, &unused, &data_ret) == Success) {
            // don't do nitems_ret check - Qt does PropModeAppend to avoid API call for it
            if (type_ret == XA_CARDINAL && format_ret == 32 /*&& nitems_ret == 1*/) {
                p->user_time = *((long *) data_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2TransientFor) {
        p->transient_for = None;
        XGetTransientForHint(p->display, p->window, &p->transient_for);
    }
    if (dirty2 & WM2GroupLeader) {
        XWMHints *hints = XGetWMHints(p->display, p->window);
        p->window_group = None;
        if (hints) {
            if (hints->flags & WindowGroupHint)
                p->window_group = hints->window_group;
            XFree(reinterpret_cast< char* >(hints));
        }
    }
    if (dirty2 & WM2WindowClass) {
        delete[] p->class_class;
        delete[] p->class_name;
        p->class_class = NULL;
        p->class_name = NULL;
        XClassHint hint;
        if (XGetClassHint(p->display, p->window, &hint)) {
            p->class_class = strdup(hint.res_class);
            p->class_name = strdup(hint.res_name);
            XFree(hint.res_class);
            XFree(hint.res_name);
        }
    }
    if (dirty2 & WM2WindowRole) {
        delete[] p->role;
        p->role = NULL;
        if (XGetWindowProperty(p->display, p->window, wm_window_role, 0l,
                               MAX_PROP_SIZE, False, XA_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_STRING && format_ret == 8 && nitems_ret > 0) {
                p->role = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
    if (dirty2 & WM2ClientMachine) {
        delete[] p->client_machine;
        p->client_machine = NULL;
        if (XGetWindowProperty(p->display, p->window, XA_WM_CLIENT_MACHINE, 0l,
                               MAX_PROP_SIZE, False, XA_STRING, &type_ret,
                               &format_ret, &nitems_ret, &unused, &data_ret)
                == Success) {
            if (type_ret == XA_STRING && format_ret == 8 && nitems_ret > 0) {
                p->client_machine = nstrndup((const char *) data_ret, nitems_ret);
            }
            if (data_ret)
                XFree(data_ret);
        }
    }
}


NETRect NETWinInfo::iconGeometry() const
{
    return p->icon_geom;
}


unsigned long NETWinInfo::state() const
{
    return p->state;
}


NETStrut NETWinInfo::strut() const
{
    return p->strut;
}

NETExtendedStrut NETWinInfo::extendedStrut() const
{
    return p->extended_strut;
}

bool NET::typeMatchesMask(WindowType type, unsigned long mask)
{
    switch (type) {
#define CHECK_TYPE_MASK(type) \
case type: \
    if(mask & type##Mask) \
        return true; \
    break;
        CHECK_TYPE_MASK(Normal)
        CHECK_TYPE_MASK(Desktop)
        CHECK_TYPE_MASK(Dock)
        CHECK_TYPE_MASK(Toolbar)
        CHECK_TYPE_MASK(Menu)
        CHECK_TYPE_MASK(Dialog)
        CHECK_TYPE_MASK(Override)
        CHECK_TYPE_MASK(TopMenu)
        CHECK_TYPE_MASK(Utility)
        CHECK_TYPE_MASK(Splash)
#undef CHECK_TYPE_MASK
    default:
        break;
    }
    return false;
}

NET::WindowType NETWinInfo::windowType(unsigned long supported_types) const
{
    for (int i = 0;
            i < p->types.size();
            ++i) {
        // return the type only if the application supports it
        if (typeMatchesMask(p->types[ i ], supported_types))
            return p->types[ i ];
    }
    return Unknown;
}

NET::WindowType NETWinInfo::windowType() const
{
    return p->types[ 0 ];
}


const char *NETWinInfo::name() const
{
    return p->name;
}


const char *NETWinInfo::visibleName() const
{
    return p->visible_name;
}


const char *NETWinInfo::iconName() const
{
    return p->icon_name;
}


const char *NETWinInfo::visibleIconName() const
{
    return p->visible_icon_name;
}


int NETWinInfo::desktop() const
{
    return p->desktop;
}

int NETWinInfo::pid() const
{
    return p->pid;
}

Time NETWinInfo::userTime() const
{
    return p->user_time;
}

const char* NETWinInfo::startupId() const
{
    return p->startup_id;
}

unsigned long NETWinInfo::allowedActions() const
{
    return p->allowed_actions;
}

bool NETWinInfo::hasNETSupport() const
{
    return p->has_net_support;
}

Window NETWinInfo::transientFor() const
{
    return p->transient_for;
}

Window NETWinInfo::groupLeader() const
{
    return p->window_group;
}

const char* NETWinInfo::windowClassClass() const
{
    return p->class_class;
}

const char* NETWinInfo::windowClassName() const
{
    return p->class_name;
}

const char* NETWinInfo::windowRole() const
{
    return p->role;
}

const char* NETWinInfo::clientMachine() const
{
    return p->client_machine;
}

Bool NETWinInfo::handledIcons() const
{
    return p->handled_icons;
}


Window NETWinInfo::kdeSystemTrayWinFor() const
{
    return p->kde_system_tray_win_for;
}

const unsigned long* NETWinInfo::passedProperties() const
{
    return p->properties;
}

unsigned long NETWinInfo::properties() const
{
    return p->properties[ PROTOCOLS ];
}


NET::MappingState NETWinInfo::mappingState() const
{
    return p->mapping_state;
}

void NETRootInfo::virtual_hook(int, void*)
{ /*BASE::virtual_hook(id, data);*/ }

void NETWinInfo::virtual_hook(int, void*)
{ /*BASE::virtual_hook(id, data);*/ }

// Functions for X timestamp comparing. For Time being 32bit they're fairly simple
// (the #if 0 part), but on 64bit architectures Time is 64bit unsigned long,
// so there special care needs to be taken to always use only the lower 32bits.
#if 0
int NET::timestampCompare(Time time1, Time time2)   // like strcmp()
{
    if (time1 == time2)
        return 0;
    return (time1 - time2) < 0x7fffffffU ? 1 : -1;   // time1 > time2 -> 1, handle wrapping
}

Time NET::timestampDiff(Time time1, Time time2)   // returns time2 - time1
{
    // no need to handle wrapping?
    return time2 - time1;
}
#else
int NET::timestampCompare(unsigned long time1_, unsigned long time2_)   // like strcmp()
{
//FIXME
    /*
    Q_UINT32 time1 = time1_;
    Q_UINT32 time2 = time2_;
    if(time1 == time2)
    return 0;
    return Q_UINT32(time1 - time2) < 0x7fffffffU ? 1 : -1; // time1 > time2 -> 1, handle wrapping
    */
    return 0;
}

int NET::timestampDiff(unsigned long time1_, unsigned long time2_)   // returns time2 - time1
{
    // no need to handle wrapping?
    /*
    Q_UINT32 time1 = time1_;
    Q_UINT32 time2 = time2_;
    return Q_UINT32(time2 - time1);
    */
    return 0;
}
#endif


#endif
