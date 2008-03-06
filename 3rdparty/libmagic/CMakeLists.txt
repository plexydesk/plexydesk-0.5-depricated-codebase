
SET (plexymagicsrc 	magic.c)

add_library (plexymagic SHARED ${plexymagicsrc})

#SET_TARGET_PROPERTIES(plexymagic
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

if(NOT WIN32)
INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)