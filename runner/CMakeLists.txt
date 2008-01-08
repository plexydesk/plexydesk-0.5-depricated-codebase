project(kandypanel)


set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
		   ${QT_INCLUDES}
		   	   ${QT_QTOPENGL_INCLUDE_DIR}
			   	   ${QT_QTNETWORK_INCLUDE_DIR}
				   	   ${QT_QTTEST_INCLUDE_DIR}
					   ${X11_INCLUDE_DIR}
					   		 )

include_directories(${CMAKE_SOURCE_DIR}/lib/)

########### source files ###############
SET(kandypanel_PART_SRCS
	main.cpp
)



########### prepare files ###############
ADD_EXECUTABLE(kandypanel ${kandypanel_PART_SRCS})
#KDE4_ADD_PLUGIN(kandypanel ${raptor_plasmoid_PART_SRCS})


TARGET_LINK_LIBRARIES(
	kandypanel 
	qtviz
	QtWebKit
        ${QT_QTCORE_LIBRARY}
        ${QT_QTOPENGL_LIBRARY}
	${QT_AND_KDECORE_LIBS}
	)


########### install files ###############
INSTALL(TARGETS kandypanel DESTINATION /usr/loca/bin/)
#INSTALL( FILES raptor.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kicker/applets )
