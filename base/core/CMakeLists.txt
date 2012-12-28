PROJECT(plexydeskcore)
cmake_minimum_required(VERSION 2.8.9)

# These variables are usually set in the toplevel directory
# but for standalone lib building it should be reset on demand
IF (NOT LIB_MAJOR)
    SET(LIB_MAJOR 0)
ENDIF (NOT LIB_MAJOR)

IF (NOT LIB_MINOR)
    SET(LIB_MINOR 7)
ENDIF (NOT LIB_MINOR)

IF (NOT LIB_RELEASE)
    SET(LIB_RELEASE 1)
ENDIF (NOT LIB_RELEASE)

IF (NOT PLEXY_CORE_LIBRARY)
    SET(PLEXY_CORE_LIBRARY plexydeskcore)
ENDIF (NOT PLEXY_CORE_LIBRARY)

IF (NOT CMAKE_INSTALL_LIBDIR)
    INCLUDE(GNUInstallDirs)
ENDIF (NOT CMAKE_INSTALL_LIBDIR)

INCLUDE_DIRECTORIES (
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Find Qt5
IF (NOT Qt5Core_FOUND)
    FIND_PACKAGE(Qt5Core REQUIRED)
    FIND_PACKAGE(Qt5Widgets REQUIRED)
    FIND_PACKAGE(Qt5Xml REQUIRED)

    SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS})
    SET(CMAKE_CXX_FLAGS "-fPIC")

    # TODO/FIXME: -DPLEXYCORE_STANDALONE is stuff to be resolved!
    MESSAGE(WARNING "\n\nBuilding lib as a minimal standalone library. Non-functional for plexydesk now!\n\n")
    SET(PLEXYCORE_STANDALONE 1)
    ADD_DEFINITIONS(-DPLEXYCORE_STANDALONE)
ENDIF (NOT Qt5Core_FOUND)

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
    pluginloader.cpp
    pendingjob.cpp
    controllerinterface.cpp
    datasource.cpp
    )

SET(headerFiles
    abstractplugininterface.h
    abstractdesktopwidget.h
    plexy.h
    debug.h
    datasource.h
    pluginloader.h
    abstractdesktopview.h
    pendingjob.h
    desktopviewplugin.h
    controllerinterface.h
    controllerplugininterface.h
    desktopviewplugininterface.h
    dataplugininterface.h
   )

SET(MOC_SRCS
    abstractplugininterface.h
    abstractdesktopwidget.h
    datasource.h
    pluginloader.h
    abstractdesktopview.h
    pendingjob.h
    controllerinterface.h
    desktopviewplugin.h
    controllerplugininterface.h
    desktopviewplugininterface.h
    dataplugininterface.h
   )

QT5_WRAP_CPP(QT_MOC_SRCS ${MOC_SRCS})

SET(sourceFiles ${sourceFiles} ${headerFiles})

ADD_LIBRARY(${PLEXY_CORE_LIBRARY} SHARED ${sourceFiles} ${CORE_LIB_SOURCE} ${CORE_LIB_SOURCE} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(${PLEXY_CORE_LIBRARY} PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
   )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(${PLEXY_CORE_LIBRARY} ${libs})

QT5_USE_MODULES(${PLEXY_CORE_LIBRARY} Core Widgets Xml OpenGL)

INSTALL(TARGETS ${PLEXY_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
