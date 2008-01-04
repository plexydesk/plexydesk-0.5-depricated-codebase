project(kandypanel)

include_directories(${CMAKE_SOURCE_DIR}/lib/)

########### source files ###############
SET(kandypanel_PART_SRCS
	main.cpp
)



########### prepare files ###############
KDE4_ADD_EXECUTABLE(kandypanel ${kandypanel_PART_SRCS})
#KDE4_ADD_PLUGIN(kandypanel ${raptor_plasmoid_PART_SRCS})


TARGET_LINK_LIBRARIES(
	kandypanel 
	qtviz
	${QT_AND_KDECORE_LIBS}
	${KDE4_KDEUI_LIBS}
	${KDE4_KIO_LIBS}
	${KDE4_KUTILS_LIBS}
	)


########### install files ###############
INSTALL(TARGETS kandypanel DESTINATION ${BIN_INSTALL_DIR})
#INSTALL( FILES raptor.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kicker/applets )
