
SET (plexysrc 	
	vector3d.cpp
		particle.cpp 
		)

add_library (plexy SHARED ${plexysrc})

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

INSTALL(TARGETS plexy DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
