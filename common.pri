INCLUDEPATH += . .. $$top_srcdir/build

debug:build_postfix = debug

release:build_postfix = release

top_builddir = $$top_srcdir/build/$$build_postfix
top_destdir  = $$top_srcdir/bin/$$build_postfix
builddir     = $$top_builddir/$$srcdir

OBJECTS_DIR  = $$builddir/.objs
MOC_DIR      = $$builddir/.mocs
UI_DIR       = $$builddir/.uis
RCC_DIR      = $$builddir/.rccs

DESTDIR      = $$top_destdir
