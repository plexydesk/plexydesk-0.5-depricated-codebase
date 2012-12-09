TEMPLATE = subdirs

SUBDIRS += themepacks

THEMEPACK.ICON = icons/plexydesk.png
themepack.icon.files = $$THEMEPACK.ICON
themepack.icon.path = $${OUT_PWD}/../build/share/plexy
INSTALLS += themepack.icon

THEMEPACK.WIDGETS = widgets/*
themepack.widgets.files = $$THEMEPACK.WIDGETS
themepack.widgets.path = $${OUT_PWD}/../build/share/plexy/skins/widgets
INSTALLS += themepack.widgets

QMAKE_EXTRA_TARGETS += themepack.icon themepack.widgets
