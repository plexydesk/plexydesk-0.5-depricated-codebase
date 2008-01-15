PROJECT(PlexyDesktop)
#siraj@kde.org
#LGPL2

FIND_PACKAGE(Qt4 REQUIRED)
#FIND_PACKAGE(Strigi REQUIRED)
#FIND_PACKAGE(Plasma REQUIRED)
#INCLUDE (KDE4Defaults)
#INCLUDE (MacroLibrary)

SET(LIB_MAJOR 0)
SET(LIB_MINOR 1)
SET(LIB_RELEASE 1)
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

#pkg_check_modules(GTKMM gtkmm-2.4)
#pkg_check_modules(CLUTTER clutter-0.5)

INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/extensions/widgets/clock
    ${CMAKE_BINARY_DIR}/plexylib
    ${CMAKE_BINARY_DIR}/widgets/qt4
    ${CMAKE_BINARY_DIR}/widgets/gtk
    ${CMAKE_BINARY_DIR}/extensions/backdrop/classicdrop
    ${CMAKE_BINARY_DIR}/extensions/widgets/demowidget
    ${CMAKE_BINARY_DIR}/extensions/widgets/videowiget
    ${CMAKE_BINARY_DIR}/extensions/widgets/cpu
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/plexylib
    ${CMAKE_SOURCE_DIR}/widgets/qt4
    ${CMAKE_SOURCE_DIR}/widgets/gtk
    ${QT_INCLUDE_DIR}
)



add_subdirectory(plexylib)
add_subdirectory(widgets/qt4/)
add_subdirectory(extensions/backdrop/classicdrop)
add_subdirectory(extensions/widgets/demowidget)
add_subdirectory(runner)
add_subdirectory(artwork)
add_subdirectory(artwork/buttons)
#add_subdirectory(extensions/widgets/videowiget)
add_subdirectory(extensions/widgets/clock)
add_subdirectory(extensions/widgets/cpu)

