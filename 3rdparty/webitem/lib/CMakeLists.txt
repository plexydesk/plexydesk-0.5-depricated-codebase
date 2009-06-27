INCLUDE_DIRECTORIES(
		   ${WEBKIT_TRUNK}/WebKit/qt/Api/
		   ${QT_INCLUDES}
		 )


SET (webqgv_src
	qwebviewitem.cpp
    )


#QT4_AUTOMOC(MOCSRC ${webqgv_src})
qt4_automoc(qwebviewitem.cpp)
#qt4_wrap_cpp(QT_MOC_SRCS  qwebviewitem.h)

ADD_LIBRARY (webqgv SHARED  ${webqgv_src})

#LINK_DIRECTORIES ( ${WEBKIT_TRUNK}/WebKitBuild/${MODE}/lib  )
TARGET_LINK_LIBRARIES(
        webqgv
        QtWebKit
        ${QT_QTWEBKIT_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTSVG_LIBRARY}
)



INSTALL(TARGETS webqgv DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
