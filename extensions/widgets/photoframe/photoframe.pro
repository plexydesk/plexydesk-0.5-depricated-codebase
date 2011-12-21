top_srcdir  = ../../..
srcdir      = photoframe_widget

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network opengl

DEFINES += plexyphotoframe_EXPORTS

DESTDIR = $$plexyext

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/shaders

CONFIG += qt

SOURCES = imageitem.cpp \
		imageplugin.cpp \
		interface.cpp \
		pictureflow.cpp

HEADERS = imageitem.h \
		imageplugin.h \
		interface.h \
		pictureflow.h

LIBS += -lplexyshaders -lplexydeskcore

TARGET = photoframe

desktop_files.files = photoframe.desktop
desktop_files.path = $$sharedir/plexy/ext/groups

SKIN_FILES = img/bg-about.jpg \
			img/bg-content.gif \
			img/bg-content.png \
			img/bg-footer.gif \
			img/bg-hoover.png \
			img/bg-recents.gif \
			img/bg-search.gif \
			img/bg-search.png \
			img/bg-search.xcf \
			img/bg-top.gif \
			img/bg-top.png \
			img/homelink.gif \
			img/i-card.gif \
			img/i-clock.gif \
			img/i-comment.gif \
			img/i-folder.gif \
			img/i-page.gif \
			img/i-posts.gif \
			img/i-rss.gif \
			img/i-user.gif \
			img/profile.jpg \
			img/submit.gif

photoframe_skin_files.files = $$SKIN_FILES
photoframe_skin_files.path = $$sharedir/plexy/skins/default/flick

INSTALLS += photoframe_skin_files
INSTALLS += desktop_files

target.path = $${plexyext}
INSTALLS += target
