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


#ifndef   __net_wm_h
#define   __net_wm_h

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "netwm_def.h"

#include <plexy.h>
// forward declaration
struct NETRootInfoPrivate;
struct NETWinInfoPrivate;
template <class Z> class NETRArray;


/**
   Common API for root window properties/protocols.

   The NETRootInfo class provides a common API for clients and window managers
   to set/read/change properties on the root window as defined by the NET Window
   Manager Specification..

   @author Bradley T. Hughes <bhughes@trolltech.com>
   @see NET
   @see NETWinInfo
   @see KWin
 **/

class VISIBLE_SYM NETRootInfo : public NET
{
public:
    /**
        Indexes for the properties array.
        @since 3.2
     **/
    // update also NETRootInfoPrivate::properties[] size when extending this
    enum { PROTOCOLS, WINDOW_TYPES, STATES, PROTOCOLS2, ACTIONS,
           PROPERTIES_SIZE};

    /**
       Window Managers should use this constructor to create a NETRootInfo object,
       which will be used to set/update information stored on the rootWindow.
       The application role is automatically set to WindowManager
       when using this constructor.

       @param display An X11 Display struct.

       @param supportWindow The Window id of the supportWindow.  The supportWindow
       must be created by the window manager as a child of the rootWindow.  The
       supportWindow must not be destroyed until the Window Manager exits.

       @param wmName A string which should be the window manager's name (ie. "KWin"
       or "Blackbox").

       @param properties An array of elements listing all properties and protocols
       the window manager supports. The elements contain OR'ed values of constants
       from the NET base class, in the following order: [0]= NET::Property,
       [1]= NET::WindowTypeMask (not NET::WindowType!), [2]= NET::State,
       [3]= NET::Property2, [4]= NET::Action.
       In future versions, the list may be extended. In case you pass less elements,
       the missing ones will be replaced with default values.

       @param properties_size The number of elements in the properties array.

       @param screen For Window Managers that support multiple screen (ie.
       "multiheaded") displays, the screen number may be explicitly defined.  If
       this argument is omitted, the default screen will be used.

       @param doActivate true to activate the window

       @since 3.2
     **/
    NETRootInfo(Display *display, Window supportWindow, const char *wmName,
     const unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);

    /**
        @deprecated
        This constructor differs from the above one only in the way it accepts
        the list of supported properties. The properties argument is equivalent
        to the first element of the properties array in the above constructor.
     **/
    NETRootInfo(Display *display, Window supportWindow, const char *wmName,
     unsigned long properties, int screen = -1, bool doActivate = true);

    /**
       Clients should use this constructor to create a NETRootInfo object, which
       will be used to query information set on the root window. The application
       role is automatically set to Client when using this constructor.

       @param display An X11 Display struct.

       @param properties An array of elements listing all protocols the client
       is interested in. The elements contain OR'ed values of constants
       from the NET base class, in the following order: [0]= NET::Property,
       [1]= NET::Property2.

       @param properties_size The number of elements in the properties array.

       @param screen For Clients that support multiple screen (ie. "multiheaded")
       displays, the screen number may be explicitly defined. If this argument is
       omitted, the default screen will be used.

       @param doActivate true to activate the window

       @since 3.2
     **/
    NETRootInfo(Display *display, const unsigned long properties[], int properties_size, int screen = -1, bool doActivate = true);

    /**
        This constructor differs from the above one only in the way it accepts
        the list of supported properties. The properties argument is equivalent
        to the first element of the properties array in the above constructor,
        and therefore you cannot read all root window properties using it.
     **/
    NETRootInfo(Display *display, unsigned long properties, int screen = -1,
     bool doActivate = true);

    /**
       Creates a shared copy of the specified NETRootInfo object.

       @param rootinfo the NETRootInfo object to copy
     **/
    NETRootInfo(const NETRootInfo &rootinfo);

    /**
       Destroys the NETRootInfo object.
     **/
    virtual ~NETRootInfo();

    /**
       Returns the X11 Display struct used.

       @return the X11 Display
     **/
    Display *x11Display() const;

    /**
       Returns the Window id of the rootWindow.

       @return the id of the root window
     **/
    Window rootWindow() const;

    /**
       Returns the Window id of the supportWindow.

       @return the id of the support window
     **/
    Window supportWindow() const;

    /**
       Returns the name of the Window Manager.

       @return the name of the window manager
     **/
    const char *wmName() const;

    /**
       Returns the screenNumber.

       @return the screen number
     **/
    int screenNumber() const;

    /**
       Returns true if the given property is supported by the window
       manager. Note that for Client mode, NET::Supported needs
       to be passed in the properties argument for this to work.
       @since 3.2
     **/
    bool isSupported(NET::Property property) const;
    /**
       @overload
       @since 3.2
     **/
    bool isSupported(NET::Property2 property) const;
    /**
       @overload
       @since 3.2
     **/
    bool isSupported(NET::WindowType type) const;
    /**
       @overload
       @since 3.2
     **/
    bool isSupported(NET::State state) const;

    /**
       @overload
       @since 3.2
     **/
    bool isSupported(NET::Action action) const;

    /**
       In the Window Manager mode, this is equivalent to the properties
       argument passed to the constructor. In the Client mode, if
       NET::Supported was passed in the properties argument, the returned
       value is array of all protocols and properties supported
       by the Window Manager. The elements of the array are the same
       as they would be passed to the Window Manager mode constructor,
       the size is the maximum array size the constructor accepts.

       @since 3.2
     **/
    const unsigned long *supportedProperties() const;

    /**
       Returns the properties argument passed to the constructor.
       The size is the maximum array size the constructor accepts.

       @since 3.2
     **/
    // KDE4 better name?
    const unsigned long *passedProperties() const;

    /**
       @deprecated

       Returns an OR'ed list of protocols passed to the constructor.
       For the constructor used by Window Managers, this is equivalent
       to the first element of the properties argument, for the constructor
       for Clients, it's the properties argument.

       Clients willing to find out all properties and protocols supported
       by the WindowManager should use supportedProperties().

       @return an OR'ed list of protocols

       @see NET::Property
     **/
    unsigned long supported() const;

    /**
       Returns an array of Window id's, which contain all managed windows.

       @return the array of Window id's

       @see clientListCount()
     **/
    const Window *clientList() const;

    /**
       Returns the number of managed windows in clientList array.

       @return the number of managed windows in the clientList array

       @see clientList()
     **/
    int clientListCount() const;

    /**
       Returns an array of Window id's, which contain all managed windows in
       stacking order.

       @return the array of Window id's in stacking order

       @see clientListStackingCount()
     **/
    const Window *clientListStacking() const;

    /**
       Returns the number of managed windows in the clientListStacking array.

       @return the number of Window id's in the client list

       @see clientListStacking()
     **/
    int clientListStackingCount() const;

    /**
       Returns an array of Window id's, which contain all KDE system tray windows.

       @return the array of Window id's of system tray windows

       @see kdeSystemTrayWindowsCount()
     **/
    const Window *kdeSystemTrayWindows() const;

    /**
       Returns the number of windows in the kdeSystemTrayWindows array.

       @return the number of Window id's in the system tray list

       @see kdeSystemTrayWindows()
     **/
    int kdeSystemTrayWindowsCount() const;

    /**
       Returns the desktop geometry size.

       The desktop argument is ignored. Early drafts of the NET WM
       Specification were unclear about the semantics of this property.

       @param desktop the number of the desktop

       @return the size of the desktop
     **/
    NETSize desktopGeometry(int desktop) const;

    /**
       Returns the viewport of the specified desktop.

       @param desktop the number of the desktop

       @return the position of the desktop's viewport
     **/
    NETPoint desktopViewport(int desktop) const;

    /**
       Returns the workArea for the specified desktop.

       @param desktop the number of the desktop

       @return the size of the work area
     **/
    NETRect workArea(int desktop) const;

    /**
       Returns the name for the specified desktop.

       @param desktop the number of the desktop

       @return the name of the desktop
     **/
    const char *desktopName(int desktop) const;

    /**
       Returns an array of Window id's, which contain the virtual root windows.

       @return the array of Window id's

       @see virtualRootsCount()
     **/
    const Window *virtualRoots() const;

    /**
       Returns the number of window in the virtualRoots array.

       @return the number of Window id's in the virtual root array

       @see virtualRoots()
     **/
    int virtualRootsCount() const;

    /**
       Returns the number of desktops.

       @return the number of desktops
     **/
    int numberOfDesktops() const;

    /**
       Returns the current desktop.

       @return the number of the current desktop
     **/
    int currentDesktop() const;

    /**
       Returns the active (focused) window.

       @return the id of the active window
     **/
    Window activeWindow() const;

    /**
       Window Managers must call this after creating the NETRootInfo object, and
       before using any other method in the class.  This method sets initial data
       on the root window and does other post-construction duties.

       Clients must also call this after creating the object to do an initial
       data read/update.
     **/
    void activate();

    /**
       Sets the list of managed windows on the Root/Desktop window.

       @param windows The array of Window id's

       @param count The number of windows in the array
     **/
    // KDE4 'const Window*', also in the others below
    void setClientList(Window *windows, unsigned int count);

    /**
       Sets the list of managed windows in stacking order on the Root/Desktop
       window.

       @param windows The array of Window id's

       @param count The number of windows in the array.
     **/
    void setClientListStacking(Window *windows, unsigned int count);

    /**
       Sets the list of KDE system tray windows on the root window.

       @param windows The array of window id's

       @param count The number of windows in the array.
     **/
    void setKDESystemTrayWindows(Window *windows, unsigned int count);

    /**
       Sets the current desktop to the specified desktop.

       @param desktop the number of the desktop
     **/
    void setCurrentDesktop(int desktop);

    /**
       Sets the desktop geometry to the specified geometry.

       The desktop argument is ignored. Early drafts of the NET WM
       Specification were unclear about the semantics of this property.

       @param desktop the number of the desktop

       @param geometry the new size of the desktop
     **/
    void setDesktopGeometry(int desktop, const NETSize &geometry);

    /**
       Sets the viewport for the current desktop to the specified point.

       @param desktop the number of the desktop

       @param viewport the new position of the desktop's viewport
     **/
    void setDesktopViewport(int desktop, const NETPoint &viewport);

    /**
       Sets the number of desktops the the specified number.

       @param numberOfDesktops the number of desktops
     **/
    void setNumberOfDesktops(int numberOfDesktops);

    /**
       Sets the name of the specified desktop.

       @param desktop the number of the desktop

       @param desktopName the new name of the desktop
     **/
    void setDesktopName(int desktop, const char *desktopName);

    /**
       Requests that the specified window becomes the active (focused) one.

       @param window the id of the new active window
       @param src whether the request comes from normal application
          or from a pager or similar tool
       @param timestamp X server timestamp of the user action that
          caused the request
       @param active_window active window of the requesting application, if any

       @since 3.2
     **/
    void setActiveWindow(Window window, NET::RequestSource src,
     Time timestamp, Window active_window);

    /**
       Sets the active (focused) window the specified window. This should
       be used only in the window manager mode.

       @param window the if of the new active window
     **/
    void setActiveWindow(Window window);

    /**
       Sets the workarea for the specified desktop

       @param desktop the number of the desktop

       @param workArea the new work area of the desktop
     **/
    void setWorkArea(int desktop, const NETRect &workArea);

    /**
       Sets the list of virtual root windows on the root window.

       @param windows The array of Window id's

       @param count The number of windows in the array.
     **/
    void setVirtualRoots(Window *windows, unsigned int count);

    /**
     * Sets the _NET_SHOWING_DESKTOP status (whether desktop is being shown).
     * @since 3.5
     */
    void setShowingDesktop(bool showing);
    /**
     * Returns the status of _NET_SHOWING_DESKTOP.
     * @since 3.5
     */
    bool showingDesktop() const;

    /**
       Assignment operator.  Ensures that the shared data reference counts are
       correct.
     **/
    const NETRootInfo &operator=(const NETRootInfo &rootinfo);

    /**
       Clients (such as pagers/taskbars) that wish to close a window should call
       this function.  This will send a request to the Window Manager, which
       usually can usually decide how to react to such requests.

       @param window the id of the window to close
     **/
    void closeWindowRequest(Window window);

    /**
       Clients (such as pagers/taskbars) that wish to start a WMMoveResize
       (where the window manager controls the resize/movement,
       i.e. _NET_WM_MOVERESIZE) should call this function.
       This will send a request to the Window Manager.

       @param window The client window that would be resized/moved.

       @param x_root X position of the cursor relative to the root window.

       @param y_root Y position of the cursor relative to the root window.

       @param direction One of NET::Direction (see base class documentation for
       a description of the different directions).
     **/
    void moveResizeRequest(Window window, int x_root, int y_root,
     Direction direction);

    /**
       Clients (such as pagers/taskbars) that wish to move/resize a window
       using WM2MoveResizeWindow (_NET_MOVERESIZE_WINDOW) should call this function.
       This will send a request to the Window Manager. See _NET_MOVERESIZE_WINDOW
       description for details.

       @param window The client window that would be resized/moved.
       @param flags Flags specifying the operation (see _NET_MOVERESIZE_WINDOW description)
       @param x Requested X position for the window
       @param y Requested Y position for the window
       @param width Requested width for the window
       @param height Requested height for the window

       @since 3.2
     **/
    void moveResizeWindowRequest(Window window, int flags, int x, int y, int width, int height);

    /**
       Sends the _NET_RESTACK_WINDOW request.
       @since 3.3
     **/
    void restackRequest(Window window, RequestSource source, Window above, int detail, Time timestamp);
    /**
       @obsolete
       @since 3.2
     **/
    void restackRequest(Window window, Window above, int detail);

    /**
       This function takes the passed XEvent and returns an OR'ed list of
       NETRootInfo properties that have changed in the properties argument.
       The new information will be read immediately by the class.
       The elements of the properties argument are as they would be passed
       to the constructor, if the array is not large enough,
       changed properties that don't fit in it won't be listed there
       (they'll be updated in the class though).

       @param event the event
       @param properties properties that changed
       @param properties_size size of the passed properties array
       @since 3.2

     **/
    void event(XEvent *event, unsigned long *properties, int properties_size);

    /**
       This function takes the passed XEvent and returns an OR'ed list of
       NETRootInfo properties that have changed.  The new information will be
       read immediately by the class. This overloaded version returns
       only a single mask, and therefore cannot check state of all properties
       like the other variant.

       @param event the event

       @return the properties
     **/
    unsigned long event(XEvent *event);


protected:
    /**
       A Client should subclass NETRootInfo and reimplement this function when
       it wants to know when a window has been added.

       @param window the id of the window to add
     **/
    virtual void addClient(Window window) {
        Q_UNUSED(window);
    }

    /**
       A Client should subclass NETRootInfo and reimplement this function when
       it wants to know when a window has been removed.

       @param window the id of the window to remove
     **/
    virtual void removeClient(Window window) {
        Q_UNUSED(window);
    }

    /**
       A Client should subclass NETRootInfo and reimplement this function when
       it wants to know when a system tray window has been added.  This is a KDE 2.0
       extension.

       @param window the id of the window to add
     **/
    virtual void addSystemTrayWin(Window window) {
        Q_UNUSED(window);
    }

    /**
       A Client should subclass NETRootInfo and reimplement this function when
       it wants to know when a system tray window has been removed.  This is a KDE 2.0
       extension.

       @param window the id of the window to remove
     **/
    virtual void removeSystemTrayWin(Window window) {
        Q_UNUSED(window);
    }

    /**
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to change the number
       of desktops.

       @param numberOfDesktops the new number of desktops
     **/
    virtual void changeNumberOfDesktops(int numberOfDesktops) {
        Q_UNUSED(numberOfDesktops);
    }

    /**
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to change the specified
       desktop geometry.

       @param desktop the number of the desktop

       @param geom the new size
     **/
    virtual void changeDesktopGeometry(int desktop, const NETSize &geom) {
        Q_UNUSED(desktop);
        Q_UNUSED(geom);
    }

    /**
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to change the specified
       desktop viewport.

       @param desktop the number of the desktop

       @param viewport the new position of the viewport
     **/
    virtual void changeDesktopViewport(int desktop, const NETPoint &viewport) {
        Q_UNUSED(desktop);
        Q_UNUSED(viewport);
    }

    /**
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to change the current
       desktop.

       @param desktop the number of the desktop
     **/
    virtual void changeCurrentDesktop(int desktop) {
        Q_UNUSED(desktop);
    }

    /**
       @deprecated Use NETRootInfo2::changeActiveWindow() instead.
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to change the active
       (focused) window. The changeActiveWindow() method in NETRootInfo2
       should be used instead.

       @param window the id of the window to activate
     **/
    virtual void changeActiveWindow(Window window) {
        Q_UNUSED(window);
    }

    /**
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to close a window.

       @param window the id of the window to close
     **/
    virtual void closeWindow(Window window) {
        Q_UNUSED(window);
    }

    /**
       A Window Manager should subclass NETRootInfo and reimplement this function
       when it wants to know when a Client made a request to start a move/resize.

       @param window The window that wants to move/resize

       @param x_root X position of the cursor relative to the root window.

       @param y_root Y position of the cursor relative to the root window.

       @param direction One of NET::Direction (see base class documentation for
       a description of the different directions).
     **/
    virtual void moveResize(Window window, int x_root, int y_root,
     unsigned long direction) {
        Q_UNUSED(window);
        Q_UNUSED(x_root);
        Q_UNUSED(y_root);
        Q_UNUSED(direction);
    }


private:
    void update(const unsigned long[]);
    void setSupported();
    void setDefaultProperties();
    void updateSupportedProperties(Atom atom);
    Role role;

protected:
    virtual void virtual_hook(int id, void *data);
private:
    NETRootInfoPrivate *p;
    friend class NETRootInfo2;
    friend class NETRootInfo3;
};

/**
   This class is an extension of the NETRootInfo class, and exists solely
   for binary compatibility reasons (adds new virtual methods). Simply
   use it instead of NETRootInfo and override also the added virtual methods.
   @since 3.2
 */
class VISIBLE_SYM NETRootInfo2
    : public NETRootInfo
{
public:
    NETRootInfo2(Display *display, Window supportWindow, const char *wmName,
     unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);
    /**
     * @since 3.5
     */
    NETRootInfo2(Display *display, const unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);
    /**
       Sends a ping with the given timestamp to the window, using
       the _NET_WM_PING protocol.
     */
    void sendPing(Window window, Time timestamp);
protected:
    friend class NETRootInfo;
    /**
       A Window Manager should subclass NETRootInfo2 and reimplement this function
       when it wants to receive replies to the _NET_WM_PING protocol.
       @param window the window from which the reply came
       @param timestamp timestamp of the ping
     */
    virtual void gotPing(Window window, Time timestamp) {
        Q_UNUSED(window);
        Q_UNUSED(timestamp);
    }
    /**
       A Window Manager should subclass NETRootInfo2 and reimplement this function
       when it wants to know when a Client made a request to change the active
       (focused) window.

       @param window the id of the window to activate
       @param src the source from which the request came
       @param timestamp the timestamp of the user action causing this request
       @param active_window active window of the requesting application, if any
     **/
    virtual void changeActiveWindow(Window window, NET::RequestSource src,
     Time timestamp, Window active_window) {
        Q_UNUSED(window);
        Q_UNUSED(src);
        Q_UNUSED(timestamp);
        Q_UNUSED(active_window);
    }
    /**
       A Window Manager should subclass NETRootInfo2 and reimplement this function
       when it wants to know when a Client made a request to restack a window.
       See _NET_RESTACK_WINDOW for details.

       @param window the id of the window to restack
       @param above other window in the restack request
       @param detail restack detail
     **/
    virtual void restackWindow(Window window, Window above, int detail) {
        Q_UNUSED(window);
        Q_UNUSED(above);
        Q_UNUSED(detail);
    }

    /**
       A Window Manager should subclass NETRootInfo2 and reimplement this function
       when it wants to know when a pager made a request to move/resize a window.
       See _NET_MOVERESIZE_WINDOW for details.

       @param window the id of the window to more/resize
       @param flags Flags specifying the operation (see _NET_MOVERESIZE_WINDOW description)
       @param x Requested X position for the window
       @param y Requested Y position for the window
       @param width Requested width for the window
       @param height Requested height for the window
     **/
    virtual void moveResizeWindow(Window window, int flags, int x, int y, int width, int height) {
        Q_UNUSED(window);
        Q_UNUSED(flags);
        Q_UNUSED(x);
        Q_UNUSED(y);
        Q_UNUSED(width);
        Q_UNUSED(height);
    }

// no private data, use NETRootInfoPrivate
};

/**
   This class is an extension of the NETRootInfo class, and exists solely
   for binary compatibility reasons (adds new virtual methods). Simply
   use it instead of NETRootInfo and override also the added virtual methods.
   @since 3.3
 */
class VISIBLE_SYM NETRootInfo3
    : public NETRootInfo2
{
public:
    NETRootInfo3(Display *display, Window supportWindow, const char *wmName,
     unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);
    /**
     * @since 3.5
     */
    NETRootInfo3(Display *display, const unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);
    /**
       Sends a take activity message with the given timestamp to the window, using
       the _NET_WM_TAKE_ACTIVITY protocol (see the WM spec for details).
       @param window the window to which the message should be sent
       @param timestamp timestamp of the message
       @param flags arbitrary flags
     */
    void takeActivity(Window window, Time timestamp, long flags);
protected:
    friend class NETRootInfo;
    /**
       A Window Manager should subclass NETRootInfo3 and reimplement this function
       when it wants to know when a Client made a request to restack a window.
       See _NET_RESTACK_WINDOW for details.

       @param window the id of the window to restack
       @param source the source of the request
       @param above other window in the restack request
       @param detail restack detail
       @param timestamp the timestamp of the request
     **/
    virtual void restackWindow(Window window, RequestSource source,
     Window above, int detail, Time timestamp) {
        Q_UNUSED(window);
        Q_UNUSED(source);
        Q_UNUSED(above);
        Q_UNUSED(detail);
        Q_UNUSED(timestamp);
    }
    /**
       A Window Manager should subclass NETRootInfo3 and reimplement this function
       when it wants to receive replies to the _NET_WM_TAKE_ACTIVITY protocol.
       @param window the window from which the reply came
       @param timestamp timestamp of the ping
       @param flags flags passed in the original message
     */
    virtual void gotTakeActivity(Window window, Time timestamp, long flags) {
        Q_UNUSED(window);
        Q_UNUSED(timestamp);
        Q_UNUSED(flags);
    }
// no private data, use NETRootInfoPrivate
};

/**
   This class is an extension of the NETRootInfo class, and exists solely
   for binary compatibility reasons (adds new virtual methods). Simply
   use it instead of NETRootInfo and override also the added virtual methods.
   @since 3.5
 */
class VISIBLE_SYM NETRootInfo4
    : public NETRootInfo3
{
public:
    NETRootInfo4(Display *display, Window supportWindow, const char *wmName,
     unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);
    NETRootInfo4(Display *display, const unsigned long properties[], int properties_size,
     int screen = -1, bool doActivate = true);

protected:
    friend class NETRootInfo;
    /**
       A Window Manager should subclass NETRootInfo2 and reimplement this function
       when it wants to know when a pager made a request to change showing the desktop.
       See _NET_SHOWING_DESKTOP for details.

       @param showing whether to activate the showing desktop mode
     **/
    virtual void changeShowingDesktop(bool showing) {
        Q_UNUSED(showing);
    }
// no private data, use NETRootInfoPrivate
};

/**
   Common API for application window properties/protocols.

   The NETWinInfo class provides a common API for clients and window managers to
   set/read/change properties on an application window as defined by the NET
   Window Manager Specification.

   @author Bradley T. Hughes <bhughes@trolltech.com>
   @see NET
   @see NETRootInfo
   @see KWin
   @see http://www.freedesktop.org/standards/wm-spec/
 **/

class VISIBLE_SYM NETWinInfo : public NET
{
public:
    /**
        Indexes for the properties array.
        @since 3.2
     **/
    // update also NETWinInfoPrivate::properties[] size when extending this
    enum { PROTOCOLS, PROTOCOLS2,
           PROPERTIES_SIZE};
    /**
       Create a NETWinInfo object, which will be used to set/read/change
       information stored on an application window.

       @param display An X11 Display struct.

       @param window The Window id of the application window.

       @param rootWindow The Window id of the root window.

       @param properties An array of elements listing all properties the client
       is interested in.The elements contain OR'ed values of constants
       from the NET base class, in the following order: [0]= NET::Property,
       [1]= NET::Property2.
       In future versions, the list may be extended. In case you pass less elements,
       the missing ones will be replaced with default values.

       @param properties_size The number of elements in the properties array.

       @param role Select the application role.  If this argument is omitted,
       the role will default to Client.

       @since 3.2
     **/
    NETWinInfo(Display *display, Window window, Window rootWindow,
     const unsigned long properties[], int properties_size,
     Role role = Client);

    /**
        This constructor differs from the above one only in the way it accepts
        the list of properties the client is interested in. The properties argument
        is equivalent to the first element of the properties array
        in the above constructor.
     **/
    NETWinInfo(Display *display, Window window,
     Window rootWindow, unsigned long properties,
     Role role = Client);

    /**
       Creates a shared copy of the specified NETWinInfo object.

       @param wininfo the NETWinInfo to copy
     **/
    NETWinInfo(const NETWinInfo &wininfo);

    /**
       Destroys the NETWinInfo object.
     **/
    virtual ~NETWinInfo();

    /**
       Assignment operator.  Ensures that the shared data reference counts are
       correct.
     **/
    const NETWinInfo &operator=(const NETWinInfo &wintinfo);

    /**
       Returns true if the window has any window type set, even if the type
       itself is not known to this implementation. Presence of a window type
       as specified by the NETWM spec is considered as the window supporting
       this specification.
       @since 3.2
       @return true if the window has support for the NETWM spec
     **/
    bool hasNETSupport() const;

    /**
       Returns the properties argument passed to the constructor.
       The size is the maximum array size the constructor accepts.

       @since 3.2
     **/
    // KDE4 better name?
    const unsigned long *passedProperties() const;

    /**
       @deprecated

       Returns an OR'ed list of protocols passed to the constructor.

       @return an OR'ed list of protocols

       @see NET::Property
     **/
    unsigned long properties() const;

    /**
       Returns the icon geometry.

       @return the geometry of the icon
     **/
    NETRect iconGeometry() const;

    /**
       Returns the state of the window (see the NET base class documentation for a
       description of the various states).

       @return the state of the window
     **/
    unsigned long state() const;

    /**
       Returns the extended (partial) strut specified by this client.
       See _NET_WM_STRUT_PARTIAL in the spec.
     **/
    NETExtendedStrut extendedStrut() const;

    /**
       @deprecated use strutPartial()
       Returns the strut specified by this client.

       @return the strut of the window
     **/
    NETStrut strut() const;

    /**
       Returns the window type for this client (see the NET base class
       documentation for a description of the various window types).
       Since clients may specify several windows types for a window
       in order to support backwards compatibility and extensions
       not available in the NETWM spec, you should specify all
       window types you application supports (see the NET::WindowTypeMask
       mask values for various window types). This method will
       return the first window type that is listed in the supported types,
       or NET::Unknown if none of the window types is supported.

       @return the type of the window
       @since 3.2
     **/
    WindowType windowType(unsigned long supported_types) const;

    /**
       @deprecated
       Returns the window type for this client (see the NET base class
       documentation for a description of the various window types).

       @return the type of the window
     **/
    WindowType windowType() const;

    /**
       Returns the name of the window in UTF-8 format.

       @return the name of the window
     **/
    const char *name() const;

    /**
       Returns the visible name as set by the window manager in UTF-8 format.

       @return the visible name of the window
     **/
    const char *visibleName() const;

    /**
       Returns the iconic name of the window in UTF-8 format. Note that this has
       nothing to do with icons, but it's for "iconic"
       representations of the window (taskbars etc.), that should be shown
       when the window is in iconic state. See description of _NET_WM_ICON_NAME
       for details.

       @return the iconic name
     **/
    const char *iconName() const;

    /**
       Returns the visible iconic name as set by the window manager in UTF-8 format.
       Note that this has nothing to do with icons, but it's for "iconic"
       representations of the window (taskbars etc.), that should be shown
       when the window is in iconic state. See description of _NET_WM_VISIBLE_ICON_NAME
       for details.

       @return the visible iconic name
     **/
    const char *visibleIconName() const;

    /**
       Returns the desktop where the window is residing.

       @return the number of the window's desktop

       @see OnAllDesktops()
     **/
    int desktop() const;

    /**
       Returns the process id for the client window.

       @return the process id of the window
     **/
    int pid() const;

    /**
       Returns whether or not this client handles icons.

       @return true if this client handles icons, false otherwise
     **/
    Bool handledIcons() const;

    /**
       Returns a Window id, telling the window manager which window we are
       representing.

       @return the window id
     **/
    Window kdeSystemTrayWinFor() const;

    /**
       Returns the mapping state for the window (see the NET base class
       documentation for a description of mapping state).

       @return the mapping state
     **/
    MappingState mappingState() const;

    /**
       Set icons for the application window.  If replace is True, then
       the specified icon is defined to be the only icon.  If replace is False,
       then the specified icon is added to a list of icons.

       @param icon the new icon

       @param replace true to replace, false to append to the list of icons
     **/
    void setIcon(NETIcon icon, Bool replace = True);

    /**
       Set the icon geometry for the application window.

       @param geometry the new icon geometry
     **/
    void setIconGeometry(NETRect geometry);

    /**
       Set the extended (partial) strut for the application window.

       @param extended_strut the new strut
     **/
    void setExtendedStrut(const NETExtendedStrut &extended_strut);

    /**
       @deprecated use setExtendedStrut()
       Set the strut for the application window.

       @param strut the new strut
     **/
    void setStrut(NETStrut strut);

    /**
       Set the state for the application window (see the NET base class documentation
       for a description of window state). Note that the constructor needs to be
       passed NET::WMState in properties even if it's only used for setting the state
       without reading it.

       @param state the name state

       @param mask the mask for the state
     **/
    void setState(unsigned long state, unsigned long mask);

    /**
       Sets the window type for this client (see the NET base class
       documentation for a description of the various window types).

       @param type the window type
     **/
    void setWindowType(WindowType type);

    /**
       Sets the name for the application window.

       @param name the new name of the window
     **/
    void setName(const char *name);

    /**
       For Window Managers only:  set the visible name (i.e. xterm, xterm <2>,
       xterm <3>, ...)

       @param visibleName the new visible name
     **/
    void setVisibleName(const char *visibleName);

    /**
       Sets the iconic name for the application window.

       @param name the new iconic name
     **/
    void setIconName(const char *name);

    /**
       For Window Managers only: set the visible iconic name (i.e. xterm, xterm <2>,
       xterm <3>, ...)

       @param name the new visible iconic name
     **/
    void setVisibleIconName(const char *name);

    /**
       Set which window the desktop is (should be) on.

       @param desktop the number of the new desktop

       @see OnAllDesktops()
     **/
    void setDesktop(int desktop);

    /**
       Set the application window's process id.

       @param pid the window's process id
     **/
    void setPid(int pid);

    /**
       Set whether this application window handles icons.

       @param handled true if the window handles icons, false otherwise
     **/
    void setHandledIcons(Bool handled);

    /**
       Set which window we are representing as a system tray window.

       @param window the window that is represented by the system tray icon
     **/
    void setKDESystemTrayWinFor(Window window);

    /**
       Set the frame decoration strut, i.e. the width of the decoration borders.

       @param strut the new strut
       @since 3.5
     **/
    void setFrameExtents(NETStrut strut);

    /**
       Set the frame decoration strut.  This is a KDE 2.0 extension to aid in
       writing pager applications.

       @param strut the new strut
     **/
    void setKDEFrameStrut(NETStrut strut);

    /**
       Returns an icon.  If width and height are passed, the icon returned will be
       the closest it can find (the next biggest).  If width and height are omitted,
       then the first icon in the list is returned.

       @param width the preferred width for the icon, -1 to ignore

       @param height the preferred height for the icon, -1 to ignore

       @return the icon
     **/
    NETIcon icon(int width = -1, int height = -1) const;

    /*
     * Sets user timestamp @p time on the window (property _NET_WM_USER_TIME).
     * The timestamp is expressed as XServer time. If a window
     * is shown with user timestamp older than the time of the last
     * user action, it won't be activated after being shown, with the special
     * value 0 meaning not to activate the window after being shown.
     * @since 3.2
     */
    void setUserTime(Time time);

    /**
     * Returns the time of last user action on the window, or -1 if not set.
     * @since 3.2
     */
    Time userTime() const;

    /*
     * Sets the startup notification id @p id on the window.
     * @since 3.2
     */
    void setStartupId(const char *startup_id);

    /**
     * Returns the startup notification id of the window.
     * @since 3.2
     */
    const char *startupId() const;

    /**
     * Sets actions that the window manager allows for the window.
     * @since 3.2
     */
    void setAllowedActions(unsigned long actions);

    /**
     * Returns actions that the window manager allows for the window.
     * @since 3.2
     */
    unsigned long allowedActions() const;

    /*
     * Returns the WM_TRANSIENT_FOR property for the window, i.e. the mainwindow
     * for this window.
     * @since 3.2
     */
    Window transientFor() const;

    /**
     * Returns the leader window for the group the window is in, if any.
     * @since 3.2
     */
    Window groupLeader() const;

    /**
     * Returns the class component of the window class for the window
     * (i.e. WM_CLASS property).
     * @since 3.3
     */
    const char *windowClassClass() const;

    /**
     * Returns the name component of the window class for the window
     * (i.e. WM_CLASS property).
     * @since 3.3
     */
    const char *windowClassName() const;

    /**
     * Returns the window role for the window (i.e. WM_WINDOW_ROLE property).
     * @since 3.3
     */
    const char *windowRole() const;

    /**
     * Returns the client machine for the window (i.e. WM_CLIENT_MACHINE property).
     * @since 3.3
     */
    const char *clientMachine() const;

    /**
       Places the window frame geometry in frame, and the application window
       geometry in window.  Both geometries are relative to the root window.

       @param frame the geometry for the frame

       @param window the geometry for the window
     **/
    void kdeGeometry(NETRect &frame, NETRect &window);

    /**
       This function takes the passed XEvent and returns an OR'ed list of
       NETWinInfo properties that have changed in the properties argument.
       The new information will be read immediately by the class.
       The elements of the properties argument are as they would be passed
       to the constructor, if the array is not large enough,
       changed properties that don't fit in it won't be listed there
       (they'll be updated in the class though).

       @param event the event
       @param properties properties that changed
       @param properties_size size of the passed properties array
       @since 3.2

     **/
    void event(XEvent *event, unsigned long *properties, int properties_size);

    /**
       This function takes the pass XEvent and returns an OR'ed list of NETWinInfo
       properties that have changed.  The new information will be read
       immediately by the class. This overloaded version returns
       only a single mask, and therefore cannot check state of all properties
       like the other variant.

       @param event the event

       @return the properties
     **/
    unsigned long event(XEvent *event);

    /**
       Sentinel value to indicate that the client wishes to be visible on
       all desktops.

       @return the value to be on all desktops
     **/
    static const int OnAllDesktops;


protected:
    /**
       A Window Manager should subclass NETWinInfo and reimplement this function when
       it wants to know when a Client made a request to change desktops (ie. move to
       another desktop).

       @param desktop the number of the desktop
     **/
    virtual void changeDesktop(int desktop) {
        Q_UNUSED(desktop);
    }

    /**
       A Window Manager should subclass NETWinInfo and reimplement this function when
       it wants to know when a Client made a request to change state (ie. to
       Shade / Unshade).

       @param state the new state

       @param mask the mask for the state
     **/
    virtual void changeState(unsigned long state, unsigned long mask) {
        Q_UNUSED(state);
        Q_UNUSED(mask);
    }

private:
    void update(const unsigned long[]);
    void updateWMState();
    void setIconInternal(NETRArray<NETIcon> &icons, int &icon_count, Atom property, NETIcon icon, Bool replace);
    NETIcon iconInternal(NETRArray<NETIcon> &icons, int icon_count, int width, int height) const;
    Role role;

protected:
    virtual void virtual_hook(int id, void *data);
private:
    NETWinInfoPrivate *p;
};


//#define KWIN_FOCUS

#endif
#endif // __net_wm_h
