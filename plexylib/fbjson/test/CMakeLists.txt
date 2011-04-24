
INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTXMLPATTERNS_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
)

SET(sourceFiles
   testjson.cpp
   ${CMAKE_SOURCE_DIR}/plexylib/fbjson/jsonhandler.cpp
)

set(headerFiles
     testjson.h
     ${CMAKE_SOURCE_DIR}/plexylib/fbjson/jsonhandler.h
)

set(QTMOC_TEST_SRCS
     testjson.h
     ${CMAKE_SOURCE_DIR}/plexylib/fbjson/jsonhandler.h
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

ADD_EXECUTABLE(plexy_json_test ${sourceFiles} ${QT_MOC_SRCS_TEST})

TARGET_LINK_LIBRARIES(
	plexy_json_test
	plexymime
	${libs}
)

INSTALL(TARGETS plexy_json_test DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
