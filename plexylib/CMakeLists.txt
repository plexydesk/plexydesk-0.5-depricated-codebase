SET(plexysrc
    vector3d.cpp
    particle.cpp
    )

ADD_LIBRARY(plexy SHARED ${plexysrc})

TARGET_LINK_LIBRARIES (
    plexy
    ${QT_QTCORE_LIBRARY}
    )

IF(NOT WIN32)
    INSTALL(TARGETS plexy DESTINATION lib)
ELSEIF(WIN32)
    INSTALL(TARGETS plexy DESTINATION bin)
ENDIF(NOT WIN32)
