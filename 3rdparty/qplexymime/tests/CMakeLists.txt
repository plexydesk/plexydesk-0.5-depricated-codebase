INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTXMLPATTERNS_INCLUDE_DIR}
    )

SET(sourceFiles
    test.cpp
    testmime.cpp
    )

SET(headerFiles
    testmime.h
    )

SET(QTMOC_SRCS
    testmime.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    )

ADD_EXECUTABLE(plexymimetest ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    plexymimetest
    plexymime
    ${libs}
    )

INSTALL(TARGETS plexymimetest DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
