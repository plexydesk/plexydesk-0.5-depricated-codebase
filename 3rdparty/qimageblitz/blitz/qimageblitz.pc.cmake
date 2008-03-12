prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@CMAKE_INSTALL_PREFIX@
libdir=@LIB_INSTALL_DIR@
includedir=@INCLUDE_INSTALL_DIR@

Name: Blitz
Description: Blitz image effect and filter library
Version: @BLITZ_LIB_VERSION_STRING@
Requires: QtGui
Libs: -L${libdir} -lqimageblitz
Cflags: -I${includedir}
