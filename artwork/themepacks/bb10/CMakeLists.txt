FILE(GLOB PNGFILES resources/*.png )
FILE(GLOB JPEGFILES resources/*.jpg )
FILE(GLOB SVGFILES resources/*.svg )
FILE(GLOB CFG main.cfg)
FILE(GLOB LAYOUT layout.xml)

# *** Check files in the final QT resources folder are unique - LINUX PLATFORMS ***
# PLEASE ADD TO $UNIQUE_RC_FILES BELOW ALL RESOURCE FILES THAT GO TO ${RESOURCES_DEST_DIR}
IF (NOT APPLE)
    IF (UNIX)
        MESSAGE(STATUS "Checking for name clashes in the QT RESOURCE folder...")
        SET(UNIQUE_RC_FILES ${PNGFILES}
                            ${JPEGFILES}
                            ${SVGFILES}
                            )
        EXECUTE_PROCESS(COMMAND ${CMAKE_SOURCE_DIR}/cmakemodules/get_duplicate_files.sh "${UNIQUE_RC_FILES}"
                        OUTPUT_VARIABLE OUT_UNIQ
                        )
        IF(NOT ${OUT_UNIQ} STREQUAL "")
            MESSAGE(FATAL_ERROR "There are name clashes(MS Windows) in the QT RESOURCE folder:\n${OUT_UNIQ}")
        ENDIF(NOT ${OUT_UNIQ} STREQUAL "")
    ENDIF (UNIX)
ENDIF (NOT APPLE)

INSTALL(FILES ${PNGFILES}  DESTINATION ${RESOURCES_DEST_DIR})
INSTALL(FILES ${JPEGFILES}  DESTINATION ${RESOURCES_DEST_DIR})
INSTALL(FILES ${SVGFILES}  DESTINATION ${RESOURCES_DEST_DIR})
INSTALL(FILES ${CFG}  DESTINATION share/plexy/themepack/default)
INSTALL(FILES ${LAYOUT}  DESTINATION share/plexy/themepack/default)
