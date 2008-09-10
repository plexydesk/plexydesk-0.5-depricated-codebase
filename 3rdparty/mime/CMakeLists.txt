
SET (mimetypesrc 	freedesktopmime.cpp)

add_library (mimetype SHARED ${mimetypesrc})

#SET_TARGET_PROPERTIES(mimetype
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

if(NOT WIN32)
INSTALL(TARGETS mimetype DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS mimetype DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)