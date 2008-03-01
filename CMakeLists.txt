PROJECT(PlexyDesktop)
#siraj@kde.org
#LGPL2
INCLUDE(CPack)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(FFMPEG REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
#FIND_PACKAGE(Strigi REQUIRED)
#FIND_PACKAGE(Plasma REQUIRED)
#INCLUDE (KDE4Defaults)
#INCLUDE (MacroLibrary)

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
        ${KDE4_INCLUDES}
        )

SET(APPLICATION_NAME "${PROJECT_NAME}")
SET(APPLICATION_MAIN_VERSION "0.${LIB_MAJOR}.${LIB_MINOR}.${LIB_RELEASE}")

CONFIGURE_FILE(
       "${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules/cmake_uninstall.cmake.in"
       "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
       IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
       "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

CONFIGURE_FILE(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h )


#pkg_check_modules(GTKMM gtkmm-2.4)
#pkg_check_modules(CLUTTER clutter-0.5)

INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/extensions/widgets/clock
    ${CMAKE_BINARY_DIR}/plexylib
    ${CMAKE_BINARY_DIR}/plexycore/qt4
    ${CMAKE_BINARY_DIR}/plexycore/gtk
    ${CMAKE_BINARY_DIR}/extensions/backdrop/classicdrop
    ${CMAKE_BINARY_DIR}/extensions/widgets/demowidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/videowiget
    ${CMAKE_BINARY_DIR}/extensions/widgets/cpu
    ${CMAKE_BINARY_DIR}/extensions/widgets/friendswidget
    ${CMAKE_BINARY_DIR}/extensions/data/flicker
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/plexylib
    ${CMAKE_SOURCE_DIR}/plexycore/qt4
    ${CMAKE_SOURCE_DIR}/widgets/gtk
    ${QT_INCLUDE_DIR}
)



add_subdirectory(plexylib)
add_subdirectory(plexycore/qt4/)
add_subdirectory(extensions/backdrop/classicdrop)

add_subdirectory(extensions/widgets/demowidget)
add_subdirectory(runner)
add_subdirectory(artwork)
add_subdirectory(artwork/buttons)
if (NOT APPLE) 
add_subdirectory(3rdparty/libmagic)
	if(FFMPEG_FOUND)
#add_subdirectory(extensions/widgets/videowiget)
	endif(FFMPEG_FOUND)
add_subdirectory(extensions/widgets/cpu)
endif(APPLE)

add_subdirectory(extensions/widgets/clock)
add_subdirectory(extensions/widgets/friendswidget)
add_subdirectory(extensions/data/flicker)

