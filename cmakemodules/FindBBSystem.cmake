# - Try to find BBSystem
# Once done this will define
#
#  BBSYSTEM_FOUND - system has BBSystem
#  BBSYSTEM_INCLUDE_DIRS - the BBSystem include directory
#  BBSYSTEM_LIBRARIES - Link these to use BBSystem
#
#  Copyright (c) 2013 Laszlo Papp <lpapp@kde.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (BBSystem_FIND_REQUIRED)
    set(_bbSystemReq "REQUIRED")
endif (BBSystem_FIND_REQUIRED)

find_package(Qt 4.8 ${_bbSystemReq})

find_path(BBSYSTEM_INCLUDE_DIR Global.hpp
  HINTS
  ENV BBSYSTEMDIR
  PATH_SUFFIXES include include/bb include/bb/system
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt/local
  /opt
  ${QNX_TARGET}/usr
)

find_library(BBSYSTEM_LIBRARY
  NAMES BBSystem bbsystem
  HINTS
  ENV BBSYSTEMDIR
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt/local
  /opt
  ${QNX_TARGET}/armv7/usr
  ${QNX_TARGET}/armle-v7/usr
)


# handle the QUIETLY and REQUIRED arguments and set BBSYSTEM_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BBSystem DEFAULT_MSG BBSYSTEM_LIBRARY BBSYSTEM_INCLUDE_DIR)

mark_as_advanced(BBSYSTEM_LIBRARY BBSYSTEM_INCLUDE_DIR)
