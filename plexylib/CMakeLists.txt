SET(plexysrc
    vector3d.cpp
    particle.cpp
    )

ADD_LIBRARY(plexy SHARED ${plexysrc})

TARGET_LINK_LIBRARIES (
    plexy
    ${QT_QTCORE_LIBRARY}
    )

INSTALL(TARGETS plexy DESTINATION ${CMAKE_INSTALL_LIBDIR})
