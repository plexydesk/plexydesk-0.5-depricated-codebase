INCLUDE_DIRECTORIES(
		   ${QT_INCLUDES}
		 )


SET (plexyjson_src
	jsonhandler.cpp
    )


#QT4_AUTOMOC(MOCSRC ${plexyjson_src})
qt4_automoc(qwebviewitem.cpp)
#qt4_wrap_cpp(QT_MOC_SRCS  qwebviewitem.h)

ADD_LIBRARY (plexyjson SHARED  ${plexyjson_src})

TARGET_LINK_LIBRARIES(
        plexyjson
        ${QT_QTSCRIPT_LIBRARY}
        ${QT_QTWEBKIT_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTSVG_LIBRARY}
)

INSTALL(TARGETS plexyjson DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)

ADD_SUBDIRECTORY(test)
