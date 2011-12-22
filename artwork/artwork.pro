top_srcdir  = ..
srcdir      = artwork

TEMPLATE = subdirs

SUBDIRS = themepacks

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

THEMEPACK.ICON = icons/plexydesk.png
themepack.icon.files = $$THEMEPACK.ICON
themepack.icon.path = $${sharedir}/plexy
INSTALLS += themepack.icon

THEMEPACK.WIDGETS = widgets/*
themepack.widgets.files = $$THEMEPACK.WIDGETS
themepack.widgets.path = $${sharedir}/plexy/skins/widgets
INSTALLS += themepack.widgets
