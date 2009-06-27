include(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
		   ${WEBKIT_TRUNK}/WebKit/qt/Api/
		   ${QT_INCLUDES}
	   	   ${QT_QTOPENGL_INCLUDE_DIR}
	   	   ${QT_QTNETWORK_INCLUDE_DIR}
	   	   ${QT_QTTEST_INCLUDE_DIR}
		   ${X11_INCLUDE_DIR}
		 )

include_directories(${CMAKE_SOURCE_DIR}/lib/)
########### source files ###############
SET(stefani_PART_SRCS
	main.cpp
	desktopview.cpp
	pictureflow.cpp
)

qt4_automoc(desktopview.cpp pictureflow.cpp)

ADD_EXECUTABLE(stefani ${stefani_PART_SRCS})

LINK_DIRECTORIES( ${WEBKIT_TRUNK}/WebKitBuild/${MODE}/lib/ )

TARGET_LINK_LIBRARIES(
	stefani
        webqgv 
	QtWebKit.so

	)


########### install files ###############
INSTALL(TARGETS stefani DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
