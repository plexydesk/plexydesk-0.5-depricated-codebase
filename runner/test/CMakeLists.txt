INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTXMLPATTERNS_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    testicon.cpp
    ${CMAKE_SOURCE_DIR}/runner/iconprovider.cpp
    ${CMAKE_SOURCE_DIR}/runner/iconjob.cpp
    ${CMAKE_SOURCE_DIR}/runner/themepackloader.cpp
)

set(headerFiles
    testicon.h
    ${CMAKE_SOURCE_DIR}/runner/iconprovider.h
    ${CMAKE_SOURCE_DIR}/runner/iconjob.h
    ${CMAKE_SOURCE_DIR}/runner/themepackloader.h
)

set(QTMOC_TEST_SRCS
    testicon.h
    ${CMAKE_SOURCE_DIR}/runner/iconprovider.h
    ${CMAKE_SOURCE_DIR}/runner/iconjob.h
    ${CMAKE_SOURCE_DIR}/runner/themepackloader.h
)

qt4_wrap_cpp(QT_MOC_SRCS_TEST ${QTMOC_TEST_SRCS})

set(sourceFiles
    ${sourceFiles}
    ${headerFiles}
)

set(libs
    ${PLEXY_CORE_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${QT_QTTEST_LIBRARY}
)

ADD_EXECUTABLE(plexy_icon_test ${sourceFiles} ${QT_MOC_SRCS_TEST})

TARGET_LINK_LIBRARIES(
    plexy_icon_test
    plexymime
    ${libs}
)

INSTALL(TARGETS plexy_icon_test DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
