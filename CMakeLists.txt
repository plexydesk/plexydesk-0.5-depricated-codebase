# Project: PlexyDesk
# License: LGPL3
# Project Author: siraj@kde.org
# CMAKE files by: siraj@kde.org
#                 PhobosK (phobosk@kbfx.net)

PROJECT(PlexyDesktop)

# Project variables that need to be defined manually
SET(LIB_MAJOR 0)
SET(LIB_MINOR 6)
SET(LIB_RELEASE 0)
SET(LIB_SUB_RELEASE 1)
SET(APPLICATION_INTERNAL_VERSION "20110703")
SET(APPLICATION_DATE "2011-07-03")
SET(APPLICATION_EXE_NAME plexydesk)
SET(PLEXY_CORE_LIBRARY plexydeskcore)


# Auto defined project variables
SET(APPLICATION_NAME "${PROJECT_NAME}")
SET(APPLICATION_MAIN_VERSION "${LIB_MAJOR}.${LIB_MINOR}.${LIB_RELEASE}.${LIB_SUB_RELEASE}")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
CMAKE_POLICY(SET CMP0003 OLD)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules)

# Needed/optional packages definitions
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(FFMPEG) #optional
FIND_PACKAGE(OpenCV) #optional
FIND_PACKAGE(QImageBlitz REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)


# Global variable CMAKE_BUILD_TYPE handling
# None (CMAKE_C_FLAGS or CMAKE_CXX_FLAGS used)
# Debug (CMAKE_C_FLAGS_DEBUG or CMAKE_CXX_FLAGS_DEBUG)
# Release (CMAKE_C_FLAGS_RELEASE or CMAKE_CXX_FLAGS_RELEASE)
# RelWithDebInfo (CMAKE_C_FLAGS_RELWITHDEBINFO or CMAKE_CXX_FLAGS_RELWITHDEBINFO
# MinSizeRel (CMAKE_C_FLAGS_MINSIZEREL or CMAKE_CXX_FLAGS_MINSIZEREL)
IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release
	    CACHE
	    STRING "Specify build type: None Debug Release RelWithDebInfo MinSizeRel."
	    FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)

# Check if we use any Debug in the final release and if so use console in Windows
# For the targets add ${WIN_APPLICATION} as second argument if you want a non
# console application like:
# example: ADD_EXECUTABLE(exe_target ${WIN_APPLICATION} ${sourceFiles})
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    MESSAGE(STATUS "Setting Debug flags ...")
    SET(WIN_APPLICATION "")
ELSE(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    # Note: the WIN32 is ignored on other platforms than Windows
    SET(WIN_APPLICATION "WIN32")
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


# This will cause cmake to include and link against the OpenGL module.
# Set all flags first and then use the QT_USE_FILE to set everything.
# Note that QT_USE_FILE also sets QT_DEFINITIONS
SET(QT_USE_OPENGL TRUE)
INCLUDE(${QT_USE_FILE})

INCLUDE(CPack)
INCLUDE(PkgConfigGetVar)

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

ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

CONFIGURE_FILE(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )
CONFIGURE_FILE(plexydesk.Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/plexydesk.Doxyfile )
CONFIGURE_FILE(make_nsis_installer.cmake ${CMAKE_CURRENT_SOURCE_DIR}/make_nsis_installer.nsi )
CONFIGURE_FILE(org.plexydesk.daemon.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.plexydesk.daemon.service)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

# *** Windows Resource file handling ***
# Put all the resource files for Windows in this section using the pattern bellow
# For every rc file a "CONFIGURE_FILE" and a "IF(MINGW)" is needed
# Then add the resource to the sources files as "resourceFiles" and use them
# in the final target to be compiled (for an example see the runner/CMakeLists.txt)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/artwork/windows/${APPLICATION_EXE_NAME}_windres.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/artwork/windows/${APPLICATION_EXE_NAME}_windres.rc")
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wally_rc.o
        COMMAND windres.exe
                  -I ${CMAKE_CURRENT_SOURCE_DIR}/artwork/windows
                  -o ${CMAKE_CURRENT_BINARY_DIR}/artwork/windows/${APPLICATION_EXE_NAME}_rc.o
                  -i ${CMAKE_CURRENT_BINARY_DIR}/artwork/windows/windres.rc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/artwork/windows
        COMMENT "Generating windows RC file ...")
ENDIF(MINGW)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
#    ${CMAKE_SOURCE_DIR}/plexylib
#    ${CMAKE_BINARY_DIR}/plexylib
    ${CMAKE_SOURCE_DIR}/base/qt4
    ${CMAKE_BINARY_DIR}/base/qt4
    ${CMAKE_SOURCE_DIR}/runner
    ${CMAKE_BINARY_DIR}/runner
    ${CMAKE_BINARY_DIR}/base/gtk
    ${CMAKE_SOURCE_DIR}/modules/libplexyirc
    ${CMAKE_BINARY_DIR}/modules/libplexyirc
    ${CMAKE_SOURCE_DIR}/modules/QtWin
    ${CMAKE_BINARY_DIR}/modules/QtWin
    ${CMAKE_SOURCE_DIR}/plexylib/fbjson
    ${CMAKE_BINARY_DIR}/plexylib/fbjson
    ${CMAKE_SOURCE_DIR}/widgets/gtk
    ${CMAKE_SOURCE_DIR}/3rdparty/lqr
    ${CMAKE_SOURCE_DIR}/3rdparty/mime
    ${CMAKE_BINARY_DIR}/3rdparty/mime
    ${CMAKE_SOURCE_DIR}/3rdparty/qplexymime
    ${CMAKE_BINARY_DIR}/3rdparty/qplexymime
    ${CMAKE_SOURCE_DIR}/3rdparty/qplexyiconprovider
    ${CMAKE_SOURCE_DIR}/3rdparty/webitem
    ${CMAKE_SOURCE_DIR}/3rdparty/webitem/lib
    ${CMAKE_BINARY_DIR}/3rdparty/webitem/lib
    ${CMAKE_SOURCE_DIR}/social
    ${CMAKE_BINARY_DIR}/social
    ${CMAKE_SOURCE_DIR}/social/libs/service
    ${CMAKE_BINARY_DIR}/social/libs/service
    ${CMAKE_SOURCE_DIR}/panel
    ${CMAKE_BINARY_DIR}/panel
#    ${CMAKE_SOURCE_DIR}/extensions/qml/folders/
#    ${CMAKE_BINARY_DIR}/extensions/qml/folders/
    ${QT_INCLUDE_DIR}
    )

IF (NOT APPLE)
    IF (UNIX)
        # *** LINUX ONLY ***
#        ADD_SUBDIRECTORY(plexywm)

        ADD_SUBDIRECTORY(panel)
        ADD_SUBDIRECTORY(social)
        ADD_SUBDIRECTORY(social/accountsmanager)

        # UI widgets
        ADD_SUBDIRECTORY(extensions/widgets/cpu)

        # Data plugins
        # ************
    ENDIF (UNIX)

    IF(NOT WIN32)
        IF(FFMPEG_FOUND)
            ADD_SUBDIRECTORY(extensions/data/videoengine)
        ENDIF(FFMPEG_FOUND)
    ENDIF(NOT WIN32)
ENDIF (NOT APPLE)

# *** ALL PLATFORMS ***
ADD_SUBDIRECTORY(base/qt4)
ADD_SUBDIRECTORY(extensions/widgets/clock)
ADD_SUBDIRECTORY(extensions/backdrop/classicdrop)
ADD_SUBDIRECTORY(data)
ADD_SUBDIRECTORY(runner)
ADD_SUBDIRECTORY(plexylib/fbjson)
ADD_SUBDIRECTORY(3rdparty/libmagic)
ADD_SUBDIRECTORY(3rdparty/mime)
ADD_SUBDIRECTORY(3rdparty/qplexymime)
ADD_SUBDIRECTORY(3rdparty/webitem)

# *** UI widgets - ALL PLATFORMS ***
# Youtube is diabled ListView is now at extensions/widgets
#ADD_SUBDIRECTORY(extensions/widgets/youtubewidget)

# *** Data plugins - ALL PLATFORMS ***
ADD_SUBDIRECTORY(extensions/data/rest)
# Youtube is diabled ListView is now at extensions/widgets
#ADD_SUBDIRECTORY(extensions/data/utube)

# *** Themes and themepacks - ALL PLATFORMS ***
ADD_SUBDIRECTORY(artwork)
ADD_SUBDIRECTORY(artwork/plexyfren_theme/default)
ADD_SUBDIRECTORY(artwork/themepacks/default)
ADD_SUBDIRECTORY(extensions/qml/folders)


# *** Removed from building - ALL PLATFORMS ***
#ADD_SUBDIRECTORY(3rdparty/cair)
#ADD_SUBDIRECTORY(extensions/widgets/demowidget)

# *** Setup CPack ***
SET(CPACK_GENERATOR "NSIS")
SET(CPACK_PACKAGE_NAME "plexydesktop")
SET(CPACK_PACKAGE_VENDOR "plexyplanet.org")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Extend your Desktop")
SET(CPACK_PACKAGE_VERSION ${APPLICATION_MAIN_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${LIB_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${LIB_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${LIB_RELEASE})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "PlexyDesk Installer")
