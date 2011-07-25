INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )

SET(exampleenginesrc
    example.cpp
    exampleinterface.cpp
		)

SET(plexicore_MOC
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/dataplugin.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/baseplugin.cpp
    )

QT4_AUTOMOC(${plexicore_MOC})

ADD_LIBRARY(exampleengine SHARED ${exampleenginesrc} ${plexicore_MOC})
QT4_AUTOMOC(${exampleenginesrc})

#SET_TARGET_PROPERTIES(plexy PROPERTIES
#                      COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})

TARGET_LINK_LIBRARIES(
	       exampleengine
	       qtviz
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTNETWORK_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS exampleengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES example.desktop DESTINATION share/plexy/ext/groups)
