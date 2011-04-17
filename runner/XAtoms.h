static char *atom_names[] = {
    "_NET_ACTIVE_WINDOW",
    "_NET_CLIENT_LIST",
    "_NET_CLOSE_WINDOW",
    "_NET_CURRENT_DESKTOP",
    "_NET_DESKTOP_GEOMETRY",
    "_NET_DESKTOP_LAYOUT",
    "_NET_DESKTOP_NAMES",
    "_NET_DESKTOP_VIEWPORT",
    "_NET_FRAME_EXTENTS",
    "_NET_MOVERESIZE_WINDOW",
    "_NET_NUMBER_OF_DESKTOPS",
    "_NET_REQUEST_FRAME_EXTENTS",
    "_NET_RESTACK_WINDOW",
    "_NET_SHOWING_DESKTOP",
    "_NET_SUPPORTED",
    "_NET_SUPPORTING_WM_CHECK",
    "_NET_VIRTUAL_ROOTS",
    "_NET_WM_ALLOWED_ACTIONS",
    "_NET_WM_DESKTOP",
    "_NET_WM_HANDLED_ICONS",
    "_NET_WM_ICON",
    "_NET_WM_ICON_GEOMETRY",
    "_NET_WM_ICON_NAME",
    "_NET_WM_MOVERESIZE",
    "_NET_WM_NAME",
    "_NET_WM_PID",
    "_NET_WM_PING",
    "_NET_WM_STATE",
    "_NET_WM_STRUT",
    "_NET_WM_STRUT_PARTIAL",
    "_NET_WM_SYNC_REQUEST",
    "_NET_WM_USER_TIME",
    "_NET_WM_VISIBLE_ICON_NAME",
    "_NET_WM_VISIBLE_NAME",
    "_NET_WM_WINDOW_TYPE",
    "_NET_WORKAREA",
    "WM_COLORMAP_WINDOWS",
    "WM_PROTOCOLS",
    "_MOTIF_WM_HINTS",
    "UTF8_STRING"
};

typedef union {
    Atom a [sizeof (atom_names) / sizeof (atom_names[0])];
    struct {
        Atom _NET_ACTIVE_WINDOW;
        Atom _NET_CLIENT_LIST;
        Atom _NET_CLOSE_WINDOW;
        Atom _NET_CURRENT_DESKTOP;
        Atom _NET_DESKTOP_GEOMETRY;
        Atom _NET_DESKTOP_LAYOUT;
        Atom _NET_DESKTOP_NAMES;
        Atom _NET_DESKTOP_VIEWPORT;
        Atom _NET_FRAME_EXTENTS;
        Atom _NET_MOVERESIZE_WINDOW;
        Atom _NET_NUMBER_OF_DESKTOPS;
        Atom _NET_REQUEST_FRAME_EXTENTS;
        Atom _NET_RESTACK_WINDOW;
        Atom _NET_SHOWING_DESKTOP;
        Atom _NET_SUPPORTED;
        Atom _NET_SUPPORTING_WM_CHECK;
        Atom _NET_VIRTUAL_ROOTS;
        Atom _NET_WM_ALLOWED_ACTIONS;
        Atom _NET_WM_DESKTOP;
        Atom _NET_WM_HANDLED_ICONS;
        Atom _NET_WM_ICON;
        Atom _NET_WM_ICON_GEOMETRY;
        Atom _NET_WM_ICON_NAME;
        Atom _NET_WM_MOVERESIZE;
        Atom _NET_WM_NAME;
        Atom _NET_WM_PID;
        Atom _NET_WM_PING;
        Atom _NET_WM_STATE;
        Atom _NET_WM_STRUT;
        Atom _NET_WM_STRUT_PARTIAL;
        Atom _NET_WM_SYNC_REQUEST;
        Atom _NET_WM_USER_TIME;
        Atom _NET_WM_VISIBLE_ICON_NAME;
        Atom _NET_WM_VISIBLE_NAME;
        Atom _NET_WM_WINDOW_TYPE;
        Atom _NET_WORKAREA;
        Atom WM_COLORMAP_WINDOWS;
        Atom WM_PROTOCOLS;
        Atom _MOTIF_WM_HINTS;
        Atom UTF8_STRING;
    } x;
} XAtoms;
