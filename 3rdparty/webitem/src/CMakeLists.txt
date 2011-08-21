INCLUDE_DIRECTORIES(
    ${WEBKIT_TRUNK}/WebKit/qt/Api
    ${X11_INCLUDE_DIR}
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

TARGET_LINK_LIBRARIES(stefani
    webqgv
    QtWebKit.so
    )


########### Install files ###############
INSTALL(TARGETS stefani DESTINATION bin)
