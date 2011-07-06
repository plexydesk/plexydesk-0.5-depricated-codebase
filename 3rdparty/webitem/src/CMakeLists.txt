INCLUDE_DIRECTORIES(
    ${WEBKIT_TRUNK}/WebKit/qt/Api
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
    )


########### Source files ###############
SET(stefani_PART_SRCS
    main.cpp
    desktopview.cpp
    pictureflow.cpp
    )

QT4_AUTOMOC(desktopview.cpp pictureflow.cpp)

ADD_EXECUTABLE(stefani ${stefani_PART_SRCS})

LINK_DIRECTORIES(${WEBKIT_TRUNK}/WebKitBuild/${MODE}/lib)

TARGET_LINK_LIBRARIES(
    stefani
    webqgv
    QtWebKit.so
    )


########### Install files ###############
INSTALL(TARGETS stefani DESTINATION bin)
