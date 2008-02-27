
SET (plexymagicsrc 	magic.c)

add_library (plexymagic SHARED ${plexymagicsrc})

#SET_TARGET_PROPERTIES(plexymagic
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
