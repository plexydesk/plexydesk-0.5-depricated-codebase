INCLUDEPATH += . .. $$top_srcdir/build

CONFIG(release, debug|release) {
    build_postfix = release
}

CONFIG(debug, debug|release) {
    build_postfix = debug
}

top_builddir     = $$top_srcdir\\output\\$$build_postfix
top_destdir      = $$top_srcdir\\build\\$$build_postfix\\bin
top_destdirlib   = $$top_srcdir\\build\\$$build_postfix\\lib
builddir         = $$top_builddir\\$$srcdir
plexyext         = $$top_destdirlib\\plexyext
sharedir         = $$top_srcdir\\build\\$$build_postfix\\share

OBJECTS_DIR      = $$builddir\\.objs
MOC_DIR          = $$builddir\\.mocs
UI_DIR           = $$builddir\\.uis
RCC_DIR          = $$builddir\\.rccs

DESTDIR          = $$top_destdir

LIBS += -L$$top_destdirlib -L$$top_destdir

DEPENDPATH += . $$top_srcdir
