PROJECT(PlexyDesktop)
#siraj@kde.org
#LGPL2


SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules)

INCLUDE(CPack)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(FFMPEG) #optional
FIND_PACKAGE(OpenGL REQUIRED)


cmake_minimum_required(VERSION 2.4)

SET(LIB_MAJOR 0)
SET(LIB_MINOR 1)
SET(LIB_RELEASE SVNBUILD)
SET(LIB_SUB_RELEASE 1)
SET(APPLICATION_INTERNAL_VERSION "20071216")
SET(APPLICATION_DATE "2008-01-01")


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

#pkg_check_modules(GTKMM gtkmm-2.4)
#pkg_check_modules(CLUTTER clutter-0.5)

INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/extensions/widgets/clock
    ${CMAKE_BINARY_DIR}/plexylib
    ${CMAKE_BINARY_DIR}/base/qt4
    ${CMAKE_BINARY_DIR}/base/gtk
    ${CMAKE_BINARY_DIR}/extensions/backdrop/classicdrop
    ${CMAKE_BINARY_DIR}/extensions/widgets/demowidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/welcome
    #    ${CMAKE_BINARY_DIR}/extensions/widgets/videowiget
    ${CMAKE_BINARY_DIR}/extensions/widgets/cpu
    ${CMAKE_BINARY_DIR}/extensions/widgets/friendswidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/imagepile
    ${CMAKE_BINARY_DIR}/extensions/widgets/youtubewidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/videoview
#	${CMAKE_BINARY_DIR}/extensions/widgets/sdlvideoview
    ${CMAKE_BINARY_DIR}/extensions/widgets/wallpaper-win32
    ${CMAKE_BINARY_DIR}/extensions/data/presence
    ${CMAKE_BINARY_DIR}/extensions/data/flicker
    ${CMAKE_BINARY_DIR}/extensions/data/rss
    ${CMAKE_BINARY_DIR}/extensions/data/irc
    ${CMAKE_BINARY_DIR}/extensions/data/utube
    ${CMAKE_BINARY_DIR}/extensions/data/videoengine 
    ${CMAKE_BINARY_DIR}/modules/libplexyirc/ 
    ${CMAKE_BINARY_DIR}/modules/QtWin/ 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/plexylib
    ${CMAKE_SOURCE_DIR}/base/qt4
    ${CMAKE_SOURCE_DIR}/widgets/gtk
    ${CMAKE_SOURCE_DIR}/3rdparty/qimageblitz/blitz/
    ${CMAKE_SOURCE_DIR}/3rdparty/lqr/
    ${CMAKE_SOURCE_DIR}/3rdparty/mime/
    ${CMAKE_SOURCE_DIR}/3rdparty/qplexymime
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

    ${QT_INCLUDE_DIR}
)



add_subdirectory(plexylib)
add_subdirectory(base/qt4/)
add_subdirectory(extensions/backdrop/classicdrop)
add_subdirectory(extensions/widgets/imagepile)
add_subdirectory(data)
add_subdirectory(social)


#add_subdirectory(extensions/widgets/demowidget)
add_subdirectory(runner)
if(NOT APPLE)
add_subdirectory(plexywm)
endif(NOT APPLE)
add_subdirectory(artwork)
add_subdirectory(3rdparty/libmagic)
add_subdirectory(3rdparty/mime)
add_subdirectory(3rdparty/qplexymime)
add_subdirectory(3rdparty/webitem)
#add_subdirectory(3rdparty/qplexyicon/provider)
add_subdirectory(3rdparty/qimageblitz)
#add_subdirectory(3rdparty/cair)
if(NOT APPLE) 
if(FFMPEG_FOUND)
add_subdirectory(extensions/data/videoengine/)
endif(FFMPEG_FOUND)
#add_subdirectory(extensions/widgets/cpu)
endif(NOT APPLE)

add_subdirectory(extensions/widgets/clock)
#add_subdirectory(extensions/widgets/welcome)
add_subdirectory(extensions/widgets/videoview)
#add_subdirectory(extensions/widgets/sdlvideoview)
#add_subdirectory(extensions/widgets/friendswidget)
add_subdirectory(extensions/widgets/youtubewidget)
#add_subdirectory(extensions/data/presence)
add_subdirectory(extensions/data/flicker)
#add_subdirectory(extensions/data/rss)
add_subdirectory(extensions/data/utube)
add_subdirectory(extensions/data/irc)
add_subdirectory(modules/libplexyirc)
#add_subdirectory(extensions/widgets/sociotwitterwidget)
if(FULL_DESKTOP)
add_subdirectory(modules/QtWin)
endif(FULL_DESKTOP)
#if(WIN32)
#add_subdirectory(extensions/widgets/wallpaper-win32)
#endif(WIN32)
