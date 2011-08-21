SET(sourceFiles
    test.cpp
    )

SET(libs
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    )

ADD_EXECUTABLE(plexy_mimetype_test ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(plexy_mimetype_test
    mimetype
    ${libs}
    )

INSTALL(TARGETS plexy_mimetype_test DESTINATION bin)
