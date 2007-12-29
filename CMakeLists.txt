#siraj@kde.org
#LGPL2

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
pkg_check_modules(CLUTTER clutter-0.5)

add_subdirectory(plexylib)

