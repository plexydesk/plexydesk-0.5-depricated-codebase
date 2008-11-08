########### configure tests ###############

# KWIN_HAVE_COMPOSITING - whether any compositing support is available
if( X11_Xcomposite_FOUND AND X11_Xdamage_FOUND )
    set( KWIN_HAVE_COMPOSITING 1 )
endif( X11_Xcomposite_FOUND AND X11_Xdamage_FOUND )

# KWIN_HAVE_OPENGL_COMPOSITING - whether OpenGL-based compositing support is available
if( KWIN_HAVE_COMPOSITING AND OPENGL_FOUND )
    set( KWIN_HAVE_OPENGL_COMPOSITING 1 )
endif( KWIN_HAVE_COMPOSITING AND OPENGL_FOUND )

# KWIN_HAVE_XRENDER_COMPOSITING - whether XRender-based compositing support is available
if( KWIN_HAVE_COMPOSITING AND X11_Xrender_FOUND AND X11_Xfixes_FOUND )
    set( KWIN_HAVE_XRENDER_COMPOSITING 1 )
endif( KWIN_HAVE_COMPOSITING AND X11_Xrender_FOUND AND X11_Xfixes_FOUND )

# safety
if( KWIN_HAVE_OPENGL_COMPOSITING OR KWIN_HAVE_XRENDER_COMPOSITING )
    # ok
else( KWIN_HAVE_OPENGL_COMPOSITING OR KWIN_HAVE_XRENDER_COMPOSITING )
    set( KWIN_HAVE_COMPOSITING ) # unset
endif( KWIN_HAVE_OPENGL_COMPOSITING OR KWIN_HAVE_XRENDER_COMPOSITING )

#macro_log_feature(KWIN_HAVE_COMPOSITING "Compositing support" "X11 Compositing support"  "http://www.x.org/" FALSE "" "XComposite and XDamage extensions and OpenGL or XRender with XFixes are required for KWin compositing support")

#macro_bool_to_01( OPENGL_FOUND KWIN_HAVE_OPENGL )

# for things that are also used by kwin libraries
configure_file(lib/kwinconfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/lib/kwinconfig.h )
# for kwin internal things
configure_file(config-kwin.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kwin.h )


########### global ###############

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/lib
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/effects
    )


add_subdirectory( lib ) 
add_subdirectory( killer )
#add_subdirectory( kcmkwin )
#add_subdirectory( clients )
#add_subdirectory( data )

if( KWIN_HAVE_COMPOSITING )
#add_subdirectory( effects )
endif( KWIN_HAVE_COMPOSITING )

########### next target ###############

set(kwin_KDEINIT_SRCS
   workspace.cpp 
   client.cpp 
   placement.cpp 
   atoms.cpp 
   utils.cpp 
   layers.cpp 
   main.cpp 
   popupinfo.cpp 
   tabbox.cpp 
   options.cpp 
   plugins.cpp 
   events.cpp 
   killwindow.cpp 
   geometrytip.cpp 
   sm.cpp 
   group.cpp 
   bridge.cpp 
   manage.cpp 
   notifications.cpp 
   activation.cpp 
   useractions.cpp 
   geometry.cpp 
   rules.cpp
   composite.cpp
   toplevel.cpp
   unmanaged.cpp
   scene.cpp
   scene_basic.cpp
   scene_xrender.cpp
   scene_opengl.cpp
   deleted.cpp
   effects.cpp
   compositingprefs.cpp
   )

qt4_add_dbus_adaptor( kwin_KDEINIT_SRCS org.kde.KWin.xml workspace.h KWin::Workspace )
add_executable(kwin ${kwin_KDEINIT_SRCS})
