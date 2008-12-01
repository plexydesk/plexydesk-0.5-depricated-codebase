
SET(plexymagicsrc
	magic.c
)

add_library (plexymagic SHARED ${plexymagicsrc})

if(NOT WIN32)
INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)