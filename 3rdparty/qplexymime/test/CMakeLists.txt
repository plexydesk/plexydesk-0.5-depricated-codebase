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

ADD_EXECUTABLE(plexy_mime_test ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(plexy_mime_test
    plexymime
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS plexy_mime_test DESTINATION bin)
