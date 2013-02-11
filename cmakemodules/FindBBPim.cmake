# - Try to find BBPim
# Once done this will define
#
#  BBPIM_FOUND - system has BBPim
#  BBPIM_INCLUDE_DIRS - the BBPim include directory
#  BBPIM_LIBRARIES - Link these to use BBPim
#
#  Copyright (c) 2013 Laszlo Papp <lpapp@kde.org>
#  Copyright (c) 2013 Siraj Razic <siraj@kde.org> 
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (BBPim_FIND_REQUIRED)
    set(_bbSystemReq "REQUIRED")
endif (BBPim_FIND_REQUIRED)

find_package(Qt 4.8 ${_bbSystemReq})

find_path(BBPIM_INCLUDE_DIR Global.hpp
  HINTS
  ENV BBPIMDIR
  PATH_SUFFIXES include include/bb include/bb/system include/bb/pim
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt/local
  /opt
  ${QNX_TARGET}/usr
)

find_library(BBPIM_LIBRARY
  NAMES BBPim bbsystem
  HINTS
  ENV BBPIMDIR
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt/local
  /opt
  ${QNX_TARGET}/armv7/usr
  ${QNX_TARGET}/armle-v7/usr
)


# handle the QUIETLY and REQUIRED arguments and set BBPIM_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BBPim DEFAULT_MSG BBPIM_LIBRARY BBPIM_INCLUDE_DIR)

mark_as_advanced(BBPIM_LIBRARY BBPIM_INCLUDE_DIR)
