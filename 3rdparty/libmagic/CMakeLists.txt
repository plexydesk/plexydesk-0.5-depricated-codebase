
IF(WIN32)
    ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)

SET(sourceFiles
    magic.c
)

SET(headerFiles
    magic.h
)

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
)

add_library (plexymagic SHARED ${sourceFiles})

if(NOT WIN32)
INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS plexymagic DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)
