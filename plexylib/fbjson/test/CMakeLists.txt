
INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTXMLPATTERNS_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
)

SET(sourceFiles
   testjson.cpp
)

SET(headerFiles
     testjson.h
)

SET(QTMOC_TEST_SRCS
     testjson.h
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

ADD_EXECUTABLE(plexy_json_test ${sourceFiles} ${QT_MOC_SRCS_TEST})

TARGET_LINK_LIBRARIES(
    plexy_json_test
    plexymime
    plexyjson
    ${libs}
)

INSTALL(TARGETS plexy_json_test DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
