
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
        $ {QT_INCLUDES}
        $ {QT_QTOPENGL_INCLUDE_DIR}
        $ {QT_QTNETWORK_INCLUDE_DIR}
        $ {QT_QTTEST_INCLUDE_DIR}
)


SET (qtvizsrc
     baserender.cpp
     desktopview.cpp
     pluginloader.cpp
     netwm.cpp
     dataplugin.cpp
     backdropplugin.cpp
     fakemime.cpp
     desktopwidget.cpp
     plexyconfig.cpp
     baseplugin.cpp
     widgetplugin.cpp
     abstractplugininterface.cpp
     viewlayer.cpp
     wellcomeitem.cpp
     frameitem.cpp
    )

add_library (qtviz SHARED $ {qtvizsrc})
QT4_AUTOMOC($ {qtvizsrc})


TARGET_LINK_LIBRARIES(
        qtviz
        qimageblitz
        $ {QT_QTGUI_LIBRARY}
        $ {OPENGL_LIBRARIES}
        $ {QT_QTCORE_LIBRARY}
        $ {QT_QTOPENGL_LIBRARY}
        $ {QT_QTSVG_LIBRARY}
)

if (NOT WIN32)
        INSTALL(TARGETS qtviz DESTINATION $ {CMAKE_INSTALL_PREFIX}/lib/)
        elseif(WIN32)
        INSTALL(TARGETS qtviz DESTINATION $ {CMAKE_INSTALL_PREFIX}/bin/)
        endif(NOT WIN32)
