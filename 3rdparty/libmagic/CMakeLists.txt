IF(MSVC)
    ADD_DEFINITIONS(/D _CRT_SECURE_NO_WARNINGS)
ENDIF(MSVC)

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

ADD_LIBRARY(plexymagic SHARED ${sourceFiles})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexymagic PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

INSTALL(TARGETS plexymagic DESTINATION lib)
