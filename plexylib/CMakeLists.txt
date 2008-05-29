
SET (plexysrc 	
	vector3d.cpp
	particle.cpp
)

add_library (plexy SHARED ${plexysrc})


TARGET_LINK_LIBRARIES (
    plexy
    ${QT_QTCORE_LIBRARY}
    )

if(NOT WIN32)
INSTALL(TARGETS plexy DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS plexy DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)
