INCLUDE_DIRECTORIES(
   ${QT_INCLUDES}
)

SET(sourceFiles
	plugin.cpp
	support.cpp
)

SET(headerFiles
	support.h
)

SET(QTMOC_SRCS
   support.h
   plugin.h
)
qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
   ${QTMOC_SRCS}
   ${sourceFiles}
   ${headerFiles}
)

SET(libs
   ${PLEXY_CORE_LIBRARY}
)

add_library (folderview  SHARED ${sourceFiles} ${QT_MOC_SRCS})
#IF(MINGW)
#    SET_TARGET_PROPERTIES(desktopview PROPERTIES
#        IMPORT_SUFFIX ".lib"
#        IMPORT_PREFIX ""
#        PREFIX ""
#    )
#ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
   folderview
   ${libs}
)

#work around for ubuntu

if (${QT_IMPORTS_DIR})
  SET(QT_IMPORTS_DIR ${CMAKE_INSTALL_PREFIX}/lib/qt4/imports/)
ENDIF(${QT_IMPORTS_DIR})

INSTALL (TARGETS folderview DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/qt4/FolderView/)
INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/qt4/FolderView/)
