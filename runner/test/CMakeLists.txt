SET(sourceFiles
    testicon.cpp
    ${CMAKE_SOURCE_DIR}/runner/iconprovider.cpp
    ${CMAKE_SOURCE_DIR}/runner/iconjob.cpp
    )

SET(headerFiles
    testicon.h
    ${CMAKE_SOURCE_DIR}/runner/iconprovider.h
    ${CMAKE_SOURCE_DIR}/runner/iconjob.h
    )

SET(QTMOC_TEST_SRCS
    testicon.h
    ${CMAKE_SOURCE_DIR}/runner/iconprovider.h
    ${CMAKE_SOURCE_DIR}/runner/iconjob.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS_TEST ${QTMOC_TEST_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${PLEXY_CORE_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${QT_QTTEST_LIBRARY}
    )

ADD_EXECUTABLE(plexy_icon_test ${sourceFiles} ${QT_MOC_SRCS_TEST})

TARGET_LINK_LIBRARIES(plexy_icon_test
    plexymime
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS plexy_icon_test DESTINATION bin)
