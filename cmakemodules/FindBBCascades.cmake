# - Try to find BBCascades
# Once done this will define
#
#  BBCASCADES_FOUND - system has BBCascades
#  BBCASCADES_INCLUDE_DIRS - the BBCascades include directory
#  BBCASCADES_LIBRARIES - Link these to use BBCascades
#
#  Copyright (c) 2013 Laszlo Papp <lpapp@kde.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (BBCascades_FIND_REQUIRED)
    set(_bbCascadesReq "REQUIRED")
endif (BBCascades_FIND_REQUIRED)

find_package(BBSystem ${_bbCascadesReq})

find_path(BBCASCADES_INCLUDE_DIR bbcascades_global.h
  HINTS
  ENV BBCASCADESDIR
  PATH_SUFFIXES include include/bb include/bb/cascades
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt/local
  /opt
  ${QNX_TARGET}/usr
)

find_library(BBCASCADES_LIBRARY
  NAMES BBCascades bbcascades
  HINTS
  ENV BBCASCADESDIR
  PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /opt/local
  /opt
  ${QNX_TARGET}/armv7/usr
  ${QNX_TARGET}/armle-v7/usr
)


# handle the QUIETLY and REQUIRED arguments and set BBCASCADES_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BBCascades  DEFAULT_MSG  BBCASCADES_LIBRARY BBCASCADES_INCLUDE_DIR)

mark_as_advanced(BBCASCADES_LIBRARY BBCASCADES_INCLUDE_DIR)
