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

#ifndef   __netwm_def_h
#define   __netwm_def_h

#include <plexy.h>
//#include <kdelibs_export.h>

/**
  Simple point class for NET classes.

  This class is a convenience class defining a point x, y.  The existence of
  this class is to keep the implementation from being dependant on a
  separate framework/library.

  NETPoint is only used by the NET API. Usually QPoint is the
  appropriate class for representing a point.

  @author Bradley T. Hughes <bhughes@trolltech.com>
**/

struct NETPoint
{
    /**
       Constructor to initialize this point to 0,0.
    **/
    NETPoint() : x(0), y(0) { }

    /*
       Public data member.
    **/
    int x, ///< x coordinate.
    y;   ///< y coordinate
};


/**
  Simple size class for NET classes.

  This class is a convenience class defining a size width by height.  The
  existence of this class is to keep the implementation from being dependant
  on a separate framework/library.

  NETSize is only used by the NET API. Usually QSize is the
  appropriate class for representing a size.

  @author Bradley T. Hughes <bhughes@trolltech.com>
**/

struct NETSize
{
    /**
       Constructor to initialize this size to 0x0
    **/
    NETSize() : width(0), height(0) { }

    /*
       Public data member.
    **/
    int width,  ///< Width.
    height;   ///< Height.
};

/**
   Simple rectangle class for NET classes.

   This class is a convenience class defining a rectangle as a point x,y with a
   size width by height.  The existence of this class is to keep the implementation
   from being dependant on a separate framework/library;

   NETRect is only used by the NET API. Usually QRect is the
   appropriate class for representing a rectangle.
**/
struct NETRect
{
    /**
       Position of the rectangle.

       @see NETPoint
    **/
    NETPoint pos;

    /**
       Size of the rectangle.

       @see NETSize
    **/
    NETSize size;
};


/**
   Simple icon class for NET classes.

   This class is a convenience class defining an icon of size width by height.
   The existence of this class is to keep the implementation from being
   dependant on a separate framework/library.

   NETIcon is only used by the NET API. Usually QIcon is the
   appropriate class for representing an icon.
**/

struct NETIcon
{
    /**
       Constructor to initialize this icon to 0x0 with data=0
    **/
    NETIcon() : data(0) { }

    /**
       Size of the icon.

       @see NETSize
    **/
    NETSize size;

    /**
       Image data for the icon.  This is an array of 32bit packed CARDINAL ARGB
       with high byte being A, low byte being B. First two bytes are width, height.
       Data is in rows, left to right and top to bottom.
    **/
    unsigned char *data;
};


/**
   Partial strut class for NET classes.

   This class is a convenience class defining a strut with left, right, top and
   bottom border values, and ranges for them.  The existence of this class is to
   keep the implementation from being dependant on a separate framework/library.
   See the _NET_WM_STRUT_PARTIAL property in the NETWM spec.
**/

struct NETExtendedStrut
{
    /**
       Constructor to initialize this struct to 0,0,0,0
    **/
    NETExtendedStrut() : left_width(0), left_start(0), left_end(0),
            right_width(0), right_start(0), right_end(0), top_width(0), top_start(0), top_end(0),
            bottom_width(0), bottom_start(0), bottom_end(0) {}

    /**
       Left border of the strut, width and range.
           **/
    int left_width, left_start, left_end;

    /**
       Right border of the strut, width and range.
    **/
    int right_width, right_start, right_end;

    /**
       Top border of the strut, width and range.
           **/
    int top_width, top_start, top_end;

    /**
       Bottom border of the strut, width and range.
           **/
    int bottom_width, bottom_start, bottom_end;

};


/**
   @deprecated use NETExtendedStrut

   Simple strut class for NET classes.

   This class is a convenience class defining a strut with left, right, top and
   bottom border values.  The existence of this class is to keep the implementation
   from being dependant on a separate framework/library. See the _NET_WM_STRUT
   property in the NETWM spec.
**/

struct NETStrut
{
    /**
       Constructor to initialize this struct to 0,0,0,0
    **/
    NETStrut() : left(0), right(0), top(0), bottom(0) { }

    /**
       Left border of the strut.
           **/
    int left;

    /**
       Right border of the strut.
    **/
    int right;

    /**
       Top border of the strut.
           **/
    int top;

    /**
       Bottom border of the strut.
           **/
    int bottom;
};


/**
  Base namespace class.

  The NET API is an implementation of the NET Window Manager Specification.

  This class is the base class for the NETRootInfo and NETWinInfo classes, which
  are used to retrieve and modify the properties of windows. To keep
  the namespace relatively clean, all enums are defined here.

  @see http://www.freedesktop.org/standards/wm-spec/
 **/

class VISIBLE_SYM NET
{
public:
    /**
       Application role.  This is used internally to determine how several action
       should be performed (if at all).

       @li Client indicates that the application is a client application.

       @li WindowManager indicates that the application is a window manager
       application.
    **/

    enum Role
    {
        Client,
        WindowManager
    };

    /**
       Window type.

       @li Unknown indicates that the window did not define a window type.

       @li Normal indicates that this is a normal, top-level window. Windows with
       Unknown window type or WM_TRANSIENT_FOR unset must be taken as this type.

       @li Desktop indicates a desktop feature. This can include a single window
       containing desktop icons with the same dimensions as the screen, allowing
       the desktop environment to have full control of the desktop, without the
       need for proxying root window clicks.

       @li Dock indicates a dock or panel feature. Typically a window manager would
       keep such windows on top of all other windows.

       @li Toolbar and Menu indicate toolbar and pinnable menu windows, respectively.

       @li Dialog indicates that this is a dialog window. If _NET_WM_WINDOW_TYPE is
       not set, then windows with WM_TRANSIENT_FOR set must be taken as this type.

       @li Override indicates that this window wants to have no Window Manager
       decorations. This is for windows that would normally use either override_redirect
       or Motif hints to give no decorations.  This is a KDE extension to the
       _NET_WM_WINDOW_TYPE mechanism.
       
       @li TopMenu indicates a toplevel menu (AKA macmenu). This is a KDE extension to the
       _NET_WM_WINDOW_TYPE mechanism.
    **/

    enum WindowType
    {
        Unknown  = -1,
        Normal   = 0,
        Desktop  = 1,
        Dock     = 2,
        Toolbar  = 3,
        Menu     = 4,
        Dialog   = 5,
        Override = 6, // NON STANDARD
        TopMenu  = 7, // NON STANDARD
        Tool     = Toolbar, // This will go away soon, COMPAT (How soon? :)
        Utility  = 8, ///< @since 3.2
        Splash   = 9 ///< @since 3.2
    };

    /**
        Values for WindowType when they should be OR'ed together, e.g.
        for the properties argument of the NETRootInfo constructor.
        @since 3.2
    **/
    enum WindowTypeMask
    {
        NormalMask   = 1<<0,
        DesktopMask  = 1<<1,
        DockMask     = 1<<2,
        ToolbarMask  = 1<<3,
        MenuMask     = 1<<4,
        DialogMask   = 1<<5,
        OverrideMask = 1<<6,
        TopMenuMask  = 1<<7,
        UtilityMask  = 1<<8,
        SplashMask   = 1<<9
    };

    // KDE4 move to WindowTypeMask
    enum { AllTypesMask = 0LU-1 };

    /**
     * Returns true if the given window type matches the mask given
     * using WindowTypeMask flags.
     */
    static bool typeMatchesMask(WindowType type, unsigned long mask);

    /**
       Window state.

       @li Modal ndicates that this is a modal dialog box. The WM_TRANSIENT_FOR hint
       MUST be set to indicate which window the dialog is a modal for, or set to
       the root window if the dialog is a modal for its window group.

       @li Sticky indicates that the Window Manager SHOULD keep the window's position
       fixed on the screen, even when the virtual desktop scrolls. Note that this is
       different from being kept on all desktops.

       @li Max{Vert,Horiz} indicates that the window is {vertically,horizontally}
       maximized.

       @li Max is more convenient than MaxVert | MaxHoriz.

       @li Shaded indicates that the window is shaded (rolled-up).

       @li SkipTaskbar indicates that a window should not be included on a taskbar.

       @li SkipPager indicates that a window should not be included on a pager.

       @li Hidden indicates that a window should not be visible on the screen (e.g. when minimised).
           Only the window manager is allowed to change it.

       @li FullScreen indicates that a window should fill the entire screen and have no window decorations.

       @li KeepAbove indicates that a window should on top of most windows (but below fullscreen windows).

       @li KeepBelow indicates that a window should be below most windows (but above any desktop windows).

       @li StaysOnTop is an obsolete name for KeepAbove.
       
       @li DemandsAttention there was an attempt to activate this window, but the window manager prevented
           this. E.g. taskbar should mark such window specially to bring user's attention to this window.
           Only the window manager is allowed to change it.

       Note that KeepAbove (StaysOnTop) and KeepBelow are meant as user preference and applications
       should avoid setting these states themselves.
    **/

    enum State
    {
        Modal        = 1<<0,
        Sticky       = 1<<1,
        MaxVert      = 1<<2,
        MaxHoriz     = 1<<3,
        Max = MaxVert | MaxHoriz,
        Shaded       = 1<<4,
        SkipTaskbar  = 1<<5,
        KeepAbove    = 1<<6, ///< @since 3.2
        StaysOnTop   = KeepAbove, // NOT STANDARD
        SkipPager    = 1<<7,
        Hidden       = 1<<8, ///< @since 3.2
        FullScreen   = 1<<9, ///< @since 3.2
        KeepBelow    = 1<<10, ///< @since 3.2
        DemandsAttention = 1<<11  ///< @since 3.2
    };

    /**
       Direction for WMMoveResize.

       When a client wants the Window Manager to start a WMMoveResize, it should
       specify one of:

       @li TopLeft
       @li Top
       @li TopRight
       @li Right
       @li BottomRight
       @li Bottom
       @li BottomLeft
       @li Left
       @li Move (for movement only)
       @li KeyboardSize (resizing via keyboard)
       @li KeyboardMove (movement via keyboard)
    **/

    enum Direction
    {
        TopLeft      = 0,
        Top          = 1,
        TopRight     = 2,
        Right        = 3,
        BottomRight  = 4,
        Bottom       = 5,
        BottomLeft   = 6,
        Left         = 7,
        Move         = 8,  // movement only
        /**
           @since 3.2
        **/
        KeyboardSize = 9,  // size via keyboard
        /**
           @since 3.2
        **/
        KeyboardMove = 10, // move via keyboard
        /**
          @since 3.5.1
        **/
        MoveResizeCancel = 11 // to ask the WM to stop moving a window
    };

    /**
       Client window mapping state.  The class automatically watches the mapping
       state of the client windows, and uses the mapping state to determine how
       to set/change different properties.

       @li Visible indicates the client window is visible to the user.

       @li Withdrawn indicates that neither the client window nor its icon is visible.

       @li Iconic indicates that the client window is not visible, but its icon is.
           This can be when the window is minimized or when it's on a different
           virtual desktop. See also NET::Hidden.
    **/

    // KDE4 aaarghl, this doesn't map correctly to Xlib #defines
    enum MappingState
    {
        Visible, // ie. NormalState
        Withdrawn,
        Iconic
    };

    /**
      Actions that can be done with a window (_NET_WM_ALLOWED_ACTIONS).
      @since 3.2
    **/
    enum Action
    {
        ActionMove           = 1<<0,
        ActionResize         = 1<<1,
        ActionMinimize       = 1<<2,
        ActionShade          = 1<<3,
        ActionStick          = 1<<4,
        ActionMaxVert        = 1<<5,
        ActionMaxHoriz       = 1<<6,
        ActionMax            = ActionMaxVert | ActionMaxHoriz,
        ActionFullScreen     = 1<<7,
        ActionChangeDesktop  = 1<<8,
        ActionClose          = 1<<9
    };

    /**
       Supported properties.  Clients and Window Managers must define which
       properties/protocols it wants to support.

       Root/Desktop window properties and protocols:

       @li Supported
       @li ClientList
       @li ClientListStacking
       @li NumberOfDesktops
       @li DesktopGeometry
       @li DesktopViewport
       @li CurrentDesktop
       @li DesktopNames
       @li ActiveWindow
       @li WorkArea
       @li SupportingWMCheck
       @li VirtualRoots
       @li CloseWindow
       @li WMMoveResize

       Client window properties and protocols:

       @li WMName
       @li WMVisibleName
       @li WMDesktop
       @li WMWindowType
       @li WMState
       @li WMStrut  (obsoleted by WM2ExtendedStrut)
       @li WMIconGeometry
       @li WMIcon
       @li WMPid
       @li WMVisibleIconName
       @li WMIconName

       ICCCM properties (provided for convenience):

       @li XAWMState

       Extended KDE protocols and properties (NOT STANDARD):

       @li KDESystemTrayWindows
       @li WMKDESystemTrayWinFor
       @li WMKDEFrameStrut
    **/

    enum Property
    {
        // root
        Supported             = 1<<0,
        ClientList            = 1<<1,
        ClientListStacking    = 1<<2,
        NumberOfDesktops      = 1<<3,
        DesktopGeometry       = 1<<4,
        DesktopViewport       = 1<<5,
        CurrentDesktop        = 1<<6,
        DesktopNames          = 1<<7,
        ActiveWindow          = 1<<8,
        WorkArea              = 1<<9,
        SupportingWMCheck     = 1<<10,
        VirtualRoots          = 1<<11,
        KDESystemTrayWindows  = 1<<12, // NOT STANDARD
        CloseWindow           = 1<<13,
        WMMoveResize          = 1<<14,

        // window
        WMName                = 1<<15,
        WMVisibleName         = 1<<16,
        WMDesktop             = 1<<17,
        WMWindowType          = 1<<18,
        WMState               = 1<<19,
        WMStrut               = 1<<20,
        WMIconGeometry        = 1<<21,
        WMIcon                = 1<<22,
        WMPid                 = 1<<23,
        WMHandledIcons        = 1<<24,
        WMPing                = 1<<25,
        WMKDESystemTrayWinFor = 1<<26, // NOT STANDARD
        XAWMState             = 1<<27, // NOT STANDARD
        WMFrameExtents        = 1<<28, ///< @since 3.5
        WMKDEFrameStrut       = WMFrameExtents, // NOT STANDARD

        // Need to be reordered
        WMIconName            = 1<<29,
        WMVisibleIconName     = 1<<30,
        WMGeometry       = 1<<31
    };

    /**
        Supported properties. This enum is an extension to NET::Property,
        because them enum is limited only to 32 bits.

        Client window properties and protocols:

        @li WM2UserTime
        @li WM2StartupId
        @li WM2TransientFor mainwindow for the window (WM_TRANSIENT_FOR)
        @li WM2GroupLeader  group leader (window_group in WM_HINTS)
        @li WM2AllowedActions
        @li WM2RestackWindow
        @li WM2MoveResizeWindow
        @li WM2ExtendedStrut
        @li WM2TemporaryRules internal, for kstart
        @li WM2WindowClass  WM_CLASS
        @li WM2WindowRole   WM_WINDOW_ROLE
        @li WM2ClientMachine WM_CLIENT_MACHINE
        
        @since 3.2

    **/
    enum Property2
    {
        WM2UserTime            = 1<<0,
        WM2StartupId           = 1<<1,
        WM2TransientFor        = 1<<2,
        WM2GroupLeader         = 1<<3,
        WM2AllowedActions      = 1<<4,
        WM2RestackWindow       = 1<<5,
        WM2MoveResizeWindow    = 1<<6,
        WM2ExtendedStrut       = 1<<7,
        WM2TakeActivity        = 1<<8,
        WM2KDETemporaryRules   = 1<<9,  // NOT STANDARD
        WM2WindowClass         = 1<<10, ///< @since 3.3
        WM2WindowRole          = 1<<11, ///< @since 3.3
        WM2ClientMachine       = 1<<12, ///< @since 3.3
        WM2ShowingDesktop      = 1<<13  ///< @since 3.5
    };

    /**
       Sentinel value to indicate that the client wishes to be visible on
       all desktops.
       @since 3.2
     **/
    enum { OnAllDesktops = -1 };

    /**
       Source of the request.
       @li FromApplication the request comes from a normal application
       @li FromTool the request comes from pager or similar tool
       @since 3.2
    **/
    // must match the values for data.l[0] field in _NET_ACTIVE_WINDOW message
    enum RequestSource
    {
        FromUnknown, // internal
        FromApplication,
        FromTool
    };

    /**
     Compares two X timestamps, taking into account wrapping and 64bit architectures.
     Return value is like with strcmp(), 0 for equal, -1 for time1 < time2, 1 for time1 > time2.
     @since 3.5.3
    */
    static int timestampCompare(unsigned long time1, unsigned long time2);
    /**
     Returns a difference of two X timestamps, time2 - time1, where time2 must be later than time1,
     as returned by timestampCompare().
     @since 3.5.3
    */
    static int timestampDiff(unsigned long time1_, unsigned long time2_);

};


#endif // __netwm_def_h
