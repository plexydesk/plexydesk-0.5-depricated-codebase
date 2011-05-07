#siraj@kde.org
#LGPL3
PROJECT(PlexyDesktop)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(FFMPEG) #optional
FIND_PACKAGE(OpenCV)
FIND_PACKAGE(QImageBlitz REQUIRED)
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

IF(MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
ENDIF(MSVC)

IF(WIN32)
  SET(FFMPEG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/win32/ffmpeg/include)
  SET(FFMPEG_LIBRARIES avcodec avdevice avformat avutil)
  SET(FFMPEG_FOUND TRUE)
ENDIF(WIN32)

SET(APPLICATION_NAME "${PROJECT_NAME}")
SET(APPLICATION_MAIN_VERSION "0.${LIB_MAJOR}.${LIB_MINOR}.${LIB_RELEASE}")
SET(PLEXY_CORE_LIBRARY plexydeskcore)

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
    ${CMAKE_BINARY_DIR}/plexylib
    ${CMAKE_BINARY_DIR}/base/qt4
    ${CMAKE_BINARY_DIR}/runner/
    ${CMAKE_BINARY_DIR}/base/gtk
    ${CMAKE_BINARY_DIR}/modules/libplexyirc/ 
    ${CMAKE_BINARY_DIR}/modules/QtWin/ 
    ${CMAKE_BINARY_DIR}/plexylib/fbjson/
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/plexylib
    ${CMAKE_SOURCE_DIR}/base/qt4
    ${CMAKE_SOURCE_DIR}/runner
    ${CMAKE_SOURCE_DIR}/widgets/gtk
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
    ${CMAKE_BINARY_DIR}/panel/
    ${CMAKE_SOURCE_DIR}/panel/
    ${QT_INCLUDE_DIR}
)

ADD_SUBDIRECTORY(base/qt4/)
ADD_SUBDIRECTORY(extensions/backdrop/classicdrop)
ADD_SUBDIRECTORY(data)
ADD_SUBDIRECTORY(runner)

IF (NOT APPLE)
IF (UNIX)
# /// Only Linux platform ///
ADD_SUBDIRECTORY(plexywm)

ADD_SUBDIRECTORY(panel)
ADD_SUBDIRECTORY(social)
ADD_SUBDIRECTORY(social/accountsmanager/)

#UI widgets
ADD_SUBDIRECTORY(extensions/widgets/cpu)

#data plugins
#////////////////////////////
ENDIF (UNIX)

IF(NOT WIN32)
IF(FFMPEG_FOUND)
ADD_SUBDIRECTORY(extensions/data/videoengine/)
ENDIF(FFMPEG_FOUND)
ENDIF(NOT WIN32)

ENDIF (NOT APPLE)

IF(WIN32)
# /// Only Winows platform ///
ADD_SUBDIRECTORY(extensions/widgets/wallpaper-win32)
# ///////////////////////////
ENDIF(WIN32)

# /// All platforms ///
ADD_SUBDIRECTORY(plexylib/fbjson)
ADD_SUBDIRECTORY(3rdparty/libmagic)
ADD_SUBDIRECTORY(3rdparty/mime)
ADD_SUBDIRECTORY(3rdparty/qplexymime)
ADD_SUBDIRECTORY(3rdparty/webitem)

#UI widgets for all platforms
ADD_SUBDIRECTORY(extensions/widgets/clock)
ADD_SUBDIRECTORY(extensions/widgets/youtubewidget/)

#data plugins for all platforms
ADD_SUBDIRECTORY(extensions/data/rest)
ADD_SUBDIRECTORY(extensions/data/utube)

ADD_SUBDIRECTORY(artwork)
ADD_SUBDIRECTORY(artwork/plexyfren_theme/default/)


# Removed from building
#ADD_SUBDIRECTORY(3rdparty/cair)
#ADD_SUBDIRECTORY(extensions/widgets/demowidget)
