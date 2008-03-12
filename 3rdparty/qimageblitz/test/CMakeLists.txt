project(blitztest)

if (WIN32)
 set(CMAKE_DEBUG_POSTFIX "d")
endif (WIN32)

if (APPLE)
 set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})
 set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif (APPLE)

include_directories(
${CMAKE_SOURCE_DIR}
${blitztest_BINARY_DIR}
${blitz_lib_SOURCE_DIR}
${QT_INCLUDE_DIR}
${QT_QTCORE_INCLUDE_DIR}
${QT_QTGUI_INCLUDE_DIR}
)

set(blitztest_SRCS main.cpp mainwindow.cpp scaledialog.cpp)

qt4_automoc(${blitztest_SRCS}) 

add_executable(blitztest ${blitztest_SRCS})

target_link_libraries(blitztest ${QT_QTCORE_LIBRARY}
${QT_QTGUI_LIBRARY} qimageblitz)

install(TARGETS blitztest DESTINATION ${BIN_INSTALL_DIR})

