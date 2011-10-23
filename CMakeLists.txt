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
SET(APPLICATION_INTERNAL_VERSION 20110803)
SET(APPLICATION_DATE 2011-08-03)
SET(APPLICATION_EXE_NAME plexydesk)
SET(PLEXY_CORE_LIBRARY plexydeskcore)


# Auto defined project variables
SET(APPLICATION_NAME "${PROJECT_NAME}")
SET(APPLICATION_MAIN_VERSION "${LIB_MAJOR}.${LIB_MINOR}.${LIB_RELEASE}")
SET(RESOURCES_DEST_DIR share/plexy/themepack/default/resources)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
CMAKE_POLICY(SET CMP0003 OLD)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules)

# Needed/optional packages definitions
FIND_PACKAGE(Qt4 4.7.0 COMPONENTS QtCore QtGui QtOpenGL QtXml QtDeclarative QtSql QtSvg REQUIRED)
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
# By default we should set it to Release. This can be overridden by a -D command
# line switch.
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
IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    MESSAGE(STATUS "Setting Debug flags ...")
    SET(WIN_APPLICATION "")
ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Note: the WIN32 is ignored on other platforms than Windows
    SET(WIN_APPLICATION "WIN32")
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")


# Check on Windows if we have used CMAKE_INSTALL_PREFIX or default it to
# outsource special ready path
# Set the install prefix
IF(WIN32)
        IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
            SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${APPLICATION_EXE_NAME}-ready-build")
        ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
        STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
ENDIF(WIN32)


INCLUDE(PkgConfigGetVar)
INCLUDE(GNUInstallDirs)

# *** QT vars handling ***
# This will cause cmake to include and link against the OpenGL module.
# Set all flags first and then use the QT_USE_FILE to set everything.
# Note that QT_USE_FILE also sets QT_DEFINITIONS
SET(QT_USE_OPENGL TRUE)
SET(QT_USE_QTMAIN TRUE)
#SET(QT_USE_QTMULTIMEDIA TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSVG TRUE)
SET(QT_USE_QTSQL TRUE)
SET(QT_USE_QTTEST TRUE)
SET(QT_USE_QTDBUS TRUE)
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTXMLPATTERNS TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(QT_USE_QTDECLARATIVE TRUE)
INCLUDE(${QT_USE_FILE})

SET(QT_IMPORTS_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/qt4/imports)

# On Windows we use a predefined QT_IMPORTS_INSTALL_DIR
IF (NOT WIN32)

# Workaround for platforms that do not provide it (Ubuntu)
IF(NOT QT_IMPORTS_DIR)
    MESSAGE("QT_IMPORTS_DIR not present...\nUsing a built in one: ${CMAKE_INSTALL_FULL_LIBDIR}/qt4/imports")
    SET(QT_IMPORTS_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/qt4/imports)
ELSE(NOT QT_IMPORTS_DIR)
    # Making QT_IMPORTS_INSTALL_DIR relative. This is a dirty hack but should work
    STRING(REGEX REPLACE "^/usr/" "" QT_IMPORTS_INSTALL_DIR "${QT_IMPORTS_DIR}")
    MESSAGE(STATUS "Relative QT Imports path set to: ${QT_IMPORTS_INSTALL_DIR}")
ENDIF(NOT QT_IMPORTS_DIR)

ENDIF(NOT WIN32)

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

CONFIGURE_FILE(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )
CONFIGURE_FILE(plexydesk.Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/plexydesk.Doxyfile )
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/runner/plexydesk.cmake.desktop"
    "${CMAKE_CURRENT_BINARY_DIR}/runner/plexydesk.desktop"
    @ONLY)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/dist/windows/make_nsis_installer.cmake.nsi"
    "${CMAKE_CURRENT_BINARY_DIR}/dist/windows/make_nsis_installer.nsi"
    @ONLY)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# *** Windows Resource file handling ***
# Put all the resource files for Windows in this section using the pattern bellow
# For every rc file a "CONFIGURE_FILE" and a "IF(MINGW)" is needed
# Then add the resource to the sources files as "resourceFiles" and use them
# in the final target to be compiled (for an example see the runner/CMakeLists.txt)
CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/dist/windows/${APPLICATION_EXE_NAME}_windres.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/dist/windows/${APPLICATION_EXE_NAME}_windres.rc")
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wally_rc.o
        COMMAND windres.exe
                  -I ${CMAKE_CURRENT_SOURCE_DIR}/dist/windows
                  -o ${CMAKE_CURRENT_BINARY_DIR}/dist/windows/${APPLICATION_EXE_NAME}_rc.o
                  -i ${CMAKE_CURRENT_BINARY_DIR}/dist/windows/windres.rc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/windows
        COMMENT "Generating windows RC file ...")
ENDIF(MINGW)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/base/qt4
    ${CMAKE_BINARY_DIR}/base/qt4
    ${CMAKE_SOURCE_DIR}/base/qt4/shaders
    ${CMAKE_BINARY_DIR}/base/qt4/shaders
    ${CMAKE_SOURCE_DIR}/runner
    ${CMAKE_BINARY_DIR}/runner
    ${CMAKE_SOURCE_DIR}/modules/libplexyirc
    ${CMAKE_BINARY_DIR}/modules/libplexyirc
    ${CMAKE_SOURCE_DIR}/modules/fbjson
    ${CMAKE_BINARY_DIR}/modules/fbjson
    ${CMAKE_SOURCE_DIR}/3rdparty/mime
    ${CMAKE_BINARY_DIR}/3rdparty/mime
    ${CMAKE_SOURCE_DIR}/3rdparty/qplexymime
    ${CMAKE_BINARY_DIR}/3rdparty/qplexymime
    ${CMAKE_SOURCE_DIR}/3rdparty/webitem
    ${CMAKE_BINARY_DIR}/3rdparty/webitem
    ${CMAKE_SOURCE_DIR}/3rdparty/webitem/lib
    ${CMAKE_BINARY_DIR}/3rdparty/webitem/lib
    ${CMAKE_SOURCE_DIR}/social
    ${CMAKE_BINARY_DIR}/social
    ${CMAKE_SOURCE_DIR}/social/libs/service
    ${CMAKE_BINARY_DIR}/social/libs/service
    ${CMAKE_SOURCE_DIR}/panel
    ${CMAKE_BINARY_DIR}/panel
    ${CMAKE_SOURCE_DIR}/dist/windows
    ${CMAKE_BINARY_DIR}/dist/windows
    ${QT_INCLUDE_DIR}
    )

IF (NOT APPLE)
    IF (UNIX)
        # *** LINUX ONLY ***
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
#            ADD_SUBDIRECTORY(extensions/data/videoengine)
        ENDIF(FFMPEG_FOUND)
    ENDIF(NOT WIN32)
ENDIF (NOT APPLE)

IF(WIN32)
    ADD_SUBDIRECTORY(dist/windows)
ENDIF(WIN32)

# *** ALL PLATFORMS ***
ADD_SUBDIRECTORY(3rdparty/mime)
ADD_SUBDIRECTORY(3rdparty/qplexymime)
ADD_SUBDIRECTORY(3rdparty/qt/folderlistmodel)
ADD_SUBDIRECTORY(3rdparty/webitem)
ADD_SUBDIRECTORY(base/qt4)
ADD_SUBDIRECTORY(extensions/widgets/clock)
ADD_SUBDIRECTORY(extensions/widgets/photoframe)
ADD_SUBDIRECTORY(extensions/widgets/folderwidget)
ADD_SUBDIRECTORY(extensions/facebook/authwin)
ADD_SUBDIRECTORY(extensions/backdrop/classicdrop)
ADD_SUBDIRECTORY(runner)
ADD_SUBDIRECTORY(modules/fbjson)

# *** UI widgets - ALL PLATFORMS ***
# Youtube is diabled ListView is now at extensions/widgets
#ADD_SUBDIRECTORY(extensions/widgets/youtubewidget)

# *** Data plugins - ALL PLATFORMS ***
ADD_SUBDIRECTORY(extensions/data/rest)
# Youtube is diabled ListView is now at extensions/widgets
#ADD_SUBDIRECTORY(extensions/data/utube)

# *** Themes and themepacks - ALL PLATFORMS ***
ADD_SUBDIRECTORY(artwork)


# *** Removed from building - ALL PLATFORMS ***
#ADD_SUBDIRECTORY(3rdparty/cair)
#ADD_SUBDIRECTORY(extensions/widgets/demowidget)
