ADD_SUBDIRECTORY(cpu)

SET(plexycpusrc
    cpu.cpp
    cpuwidget.cpp
    )

SET(headerFiles
    cpu.h
    cpuwidget.h
    cpuinterface.h
    )

SET(QTMOC_SRCS
    cpu.h
    cpuwidget.h
    cpuinterface.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${plexycpusrc}
    ${QT_MOC_SRCS}
    )

SET(libs
    ${PLEXY_CORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

ADD_LIBRARY(plexycpu SHARED ${sourceFiles})

TARGET_LINK_LIBRARIES(plexycpu
    ${libs}
    )

INSTALL(TARGETS plexycpu DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES cpu.desktop DESTINATION share/plexy/ext/groups)
