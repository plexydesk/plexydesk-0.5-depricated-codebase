#siraj@kde.org
#LGPL3
PROJECT(PlexyDesktop)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(FFMPEG) #optional
FIND_PACKAGE(OpenCV)
FIND_PACKAGE(OpenGL REQUIRED)
cmake_minimum_required(VERSION 2.4)

INCLUDE(CPack)

SET(LIB_MAJOR 0)
SET(LIB_MINOR 2)
SET(LIB_RELEASE SVNBUILD)
SET(LIB_SUB_RELEASE 1)
SET(APPLICATION_INTERNAL_VERSION "20071216")
SET(APPLICATION_DATE "2011-01-15")


ADD_DEFINITIONS (${QT_DEFINITIONS})
INCLUDE_DIRECTORIES (
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        )

IF(MSVC80)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
ENDIF(MSVC80)

IF(WIN32)
  SET(FFMPEG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/win32/ffmpeg/include)
  SET(FFMPEG_LIBRARIES avcodec avdevice avformat avutil)
  SET(FFMPEG_FOUND TRUE)
ENDIF(WIN32)

SET(APPLICATION_NAME "${PROJECT_NAME}")
SET(APPLICATION_MAIN_VERSION "0.${LIB_MAJOR}.${LIB_MINOR}.${LIB_RELEASE}")


CONFIGURE_FILE(
       "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules/cmake_uninstall.cmake.in"
       "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
       IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
       "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

CONFIGURE_FILE(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )
CONFIGURE_FILE(plexydesk.Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/plexydesk.Doxyfile )
CONFIGURE_FILE(org.plexydesk.daemon.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.plexydesk.daemon.service)

INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/extensions/widgets/clock
    ${CMAKE_BINARY_DIR}/plexylib
    ${CMAKE_BINARY_DIR}/base/qt4
    ${CMAKE_BINARY_DIR}/base/gtk
    ${CMAKE_BINARY_DIR}/extensions/backdrop/classicdrop
    ${CMAKE_BINARY_DIR}/extensions/widgets/demowidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/welcome
    ${CMAKE_BINARY_DIR}/extensions/widgets/cpu
    ${CMAKE_BINARY_DIR}/extensions/widgets/friendswidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/imagepile
    ${CMAKE_BINARY_DIR}/extensions/widgets/youtubewidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/videoview
    ${CMAKE_BINARY_DIR}/extensions/widgets/wallpaper-win32
    ${CMAKE_BINARY_DIR}/extensions/data/presence
    ${CMAKE_BINARY_DIR}/extensions/data/flicker
    ${CMAKE_BINARY_DIR}/extensions/data/rss
    ${CMAKE_BINARY_DIR}/extensions/data/irc
    ${CMAKE_BINARY_DIR}/extensions/data/utube
    ${CMAKE_BINARY_DIR}/extensions/data/videoengine 
    ${CMAKE_BINARY_DIR}/modules/libplexyirc/ 
    ${CMAKE_BINARY_DIR}/modules/QtWin/ 
    ${CMAKE_BINARY_DIR}/plexylib/fbjson/
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/plexylib
    ${CMAKE_SOURCE_DIR}/base/qt4
    ${CMAKE_SOURCE_DIR}/widgets/gtk
    ${CMAKE_SOURCE_DIR}/3rdparty/qimageblitz/blitz/
    ${CMAKE_SOURCE_DIR}/3rdparty/lqr/
    ${CMAKE_SOURCE_DIR}/3rdparty/mime/
    ${CMAKE_SOURCE_DIR}/3rdparty/qplexymime
    ${CMAKE_SOURCE_DIR}/plexylib/fbjson/
    ${CMAKE_BINARY_DIR}/3rdparty/qplexymime
    ${CMAKE_SOURCE_DIR}/modules/libplexyirc/
    ${CMAKE_SOURCE_DIR}/modules/QtWin/
    ${CMAKE_BINARY_DIR}/3rdparty/mime/
    ${CMAKE_SOURCE_DIR}/3rdparty/qplexyiconprovider/
    ${CMAKE_SOURCE_DIR}/3rdparty/webitem/
    ${CMAKE_SOURCE_DIR}/3rdparty/webitem/lib/
    ${CMAKE_BINARY_DIR}/3rdparty/webitem/lib/
    ${CMAKE_BINARY_DIR}/social/
    ${CMAKE_SOURCE_DIR}/social/
    ${CMAKE_SOURCE_DIR}/social/libs/service/
    ${CMAKE_BINARY_DIR}/social/libs/service/
    ${QT_INCLUDE_DIR}
)

ADD_SUBDIRECTORY(base/qt4/)
ADD_SUBDIRECTORY(extensions/backdrop/classicdrop)
ADD_SUBDIRECTORY(extensions/widgets/imagepile)

ADD_SUBDIRECTORY(data)
IF (NOT APPLE)
IF (UNIX)
ADD_SUBDIRECTORY(social)
ADD_SUBDIRECTORY(social/accountsmanager/)
ENDIF (UNIX)
ENDIF (NOT APPLE)

#add_subdirectory(extensions/widgets/demowidget)
ADD_SUBDIRECTORY(plexylib/fbjson)
ADD_SUBDIRECTORY(runner)
ADD_SUBDIRECTORY(plexyfrens)
IF(NOT APPLE)
ADD_SUBDIRECTORY(plexywm)
ENDIF(NOT APPLE)
ADD_SUBDIRECTORY(artwork)
ADD_SUBDIRECTORY(artwork/plexyfren_theme/default/)
ADD_SUBDIRECTORY(3rdparty/libmagic)
ADD_SUBDIRECTORY(3rdparty/mime)
ADD_SUBDIRECTORY(3rdparty/qplexymime)
ADD_SUBDIRECTORY(3rdparty/webitem)
ADD_SUBDIRECTORY(3rdparty/qimageblitz)
#ADD_SUBDIRECTORY(3rdparty/cair)
IF(NOT APPLE) 
IF(FFMPEG_FOUND)
ADD_SUBDIRECTORY(extensions/data/videoengine/)
ENDIF(FFMPEG_FOUND)
#ADD_SUBDIRECTORY(extensions/widgets/cpu) // TODO : port this
ENDIF(NOT APPLE)

ADD_SUBDIRECTORY(extensions/widgets/clock)
ADD_SUBDIRECTORY(extensions/widgets/videoview)
ADD_SUBDIRECTORY(extensions/widgets/youtubewidget)
ADD_SUBDIRECTORY(extensions/data/flicker)
ADD_SUBDIRECTORY(extensions/data/utube)
ADD_SUBDIRECTORY(modules/libplexyirc)
ADD_SUBDIRECTORY(extensions/data/rest/)
ADD_SUBDIRECTORY(extensions/widgets/twit/)

#facebook
ADD_SUBDIRECTORY(social)
ADD_SUBDIRECTORY(social/libs/service)
ADD_SUBDIRECTORY(extensions/facebook/authwin)
ADD_SUBDIRECTORY(extensions/facebook/welcome)

#unported
#ADD_SUBDIRECTORY(extensions/widgets/friendswidget) // TODO : port this
#ADD_SUBDIRECTORY(extensions/data/presence) // TODO : port this
#ADD_SUBDIRECTORY(extensions/data/rss) // TODO : port this and clean ups
#ADD_SUBDIRECTORY(extensions/data/irc) // TODO : port this move it out of main
#ADD_SUBDIRECTORY(extensions/widgets/sociotwitterwidget) // TODO : port this

#windows
IF(FULL_DESKTOP)
ADD_SUBDIRECTORY(modules/QtWin)
ENDIF(FULL_DESKTOP)
#IF(WIN32)
#ADD_SUBDIRECTORY(extensions/widgets/wallpaper-win32) // TODO : port this
#ENDIF(WIN32)
IF(OpenCV_FOUND)
 
ADD_SUBDIRECTORY(extensions/data/cvwebcam/)
#ADD_SUBDIRECTORY(headtrack) // TODO : remove
ENDIF(OpenCV_FOUND)
#social // TODO : review o.o
