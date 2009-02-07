# Try to find the Qt binding of the Telepathy library
# TELEPATHY_QT_FOUND - system has Telepathy-Qt
# TELEPATHY_QT_INCLUDE_DIR - the Telepathy-Qt include directory
# TELEPATHY_QT_LIBRARIES - Link these to use Telepathy-Qt

# Copyright (c) 2008, Allen Winter <winter@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set (TELEPATHY_QT4_FIND_REQUIRED ${TelepathyQt4_FIND_REQUIRED})
if (TELEPATHY_QT4_INCLUDE_DIR AND TELEPATHY_QT4_LIBRARIES)
  # Already in cache, be silent
  set (TELEPATHY_QT4_FIND_QUIETLY TRUE)
endif (TELEPATHY_QT4_INCLUDE_DIR AND TELEPATHY_QT4_LIBRARIES)

find_path (TELEPATHY_QT4_INCLUDE_DIR
          NAMES TelepathyQt4/Client/Channel TelepathyQt4/Client/Connection TelepathyQt4/Types
          PATHS ${CMAKE_INSTALL_PREFIX}/include/telepathy-1.0
)
find_library (TELEPATHY_QT4_LIBRARIES
             NAMES telepathy-qt4
             PATHS ${CMAKE_INSTALL_PREFIX}/lib
)

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (TELEPATHY_QT4 DEFAULT_MSG
                                   TELEPATHY_QT4_LIBRARIES TELEPATHY_QT4_INCLUDE_DIR)
