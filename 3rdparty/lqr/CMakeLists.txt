
SET (seamsrc 	
	lqr_carver.c
	lqr_carver_bias.c
	lqr_carver_list.c
	lqr_cursor.c
	lqr_gradient.c
	lqr_progress.c
	lqr_vmap.c
	lqr_vmap_list.c)

add_library (seam SHARED ${seamsrc})

#SET_TARGET_PROPERTIES(seam
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

if(NOT WIN32)
INSTALL(TARGETS seam DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS seam DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)