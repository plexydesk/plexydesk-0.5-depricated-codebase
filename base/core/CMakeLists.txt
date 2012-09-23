PROJECT(plexydeskcore)
cmake_minimum_required(VERSION 2.6)

# These variables are usually set in the toplevel directory
# but for standalone lib building it should be reset on demand
IF (NOT LIB_MAJOR)
    SET(LIB_MAJOR 0)
ENDIF (NOT LIB_MAJOR)
IF (NOT LIB_MINOR)
    SET(LIB_MINOR 6)
ENDIF (NOT LIB_MINOR)
IF (NOT LIB_RELEASE)
    SET(LIB_RELEASE 0)
ENDIF (NOT LIB_RELEASE)
IF (NOT PLEXY_CORE_LIBRARY)
    SET(PLEXY_CORE_LIBRARY plexydeskcore)
ENDIF (NOT PLEXY_CORE_LIBRARY)
IF (NOT CMAKE_INSTALL_LIBDIR)
    INCLUDE(GNUInstallDirs)
ENDIF (NOT CMAKE_INSTALL_LIBDIR)

# Find Qt4
IF (NOT QT_FOUND)
    FIND_PACKAGE(Qt4 4.7.0 COMPONENTS QtCore QtGui QtOpenGL)
    INCLUDE(${QT_USE_FILE})

    INCLUDE_DIRECTORIES (
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    # TODO/FIXME: -DPLEXYCORE_STANDALONE is stuff to be resolved!
    message(WARNING "\n\nBuilding lib as a minimal standalone library. Non-functional for plexydesk now!\n\n")
    SET(PLEXYCORE_STANDALONE 1)
    ADD_DEFINITIONS(-DPLEXYCORE_STANDALONE)
ENDIF (NOT QT_FOUND)

# do not use config.h now
ADD_DEFINITIONS(-DPLEXYPREFIX="${CMAKE_INSTALL_PREFIX}"
                -DPLEXYLIBDIR="${CMAKE_INSTALL_LIBDIR}"
                -DPLEXYQTIMPORTSDIR="${QT_IMPORTS_INSTALL_DIR}"
                -DPLEXYRESOURCESDIR="${RESOURCES_DEST_DIR}"
               )

SET(sourceFiles
    abstractplugininterface.cpp
    abstractdesktopwidget.cpp
    abstractdesktopview.cpp
    backdropplugin.cpp
    abstractsource.h
    pluginloader.cpp
    pendingjob.cpp
    controllerinterface.cpp
    )

SET(headerFiles
    abstractplugininterface.h
    abstractdesktopwidget.h
    backdropplugin.h
    abstractsource.h
    plexy.h
    debug.h
    datasource.h
    pluginloader.h
    abstractdesktopview.h
    pendingjob.h
    desktopviewplugin.h
    controllerinterface.h
   )

SET(MOC_SRCS
    abstractplugininterface.h
    abstractdesktopwidget.h
    backdropplugin.h
    abstractsource.h
    datasource.h
    pluginloader.h
    abstractdesktopview.h
    pendingjob.h
    controllerinterface.h
    desktopviewplugin.h
   )

QT4_WRAP_CPP(QT_MOC_SRCS ${MOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    ${dbus_SRCS}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${DBUS_LIB}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

ADD_LIBRARY(${PLEXY_CORE_LIBRARY} SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(${PLEXY_CORE_LIBRARY} PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS ${PLEXY_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
