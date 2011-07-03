INCLUDE(UsePkgConfig)

MACRO(PKGCONFIG_GETVAR _package _var _output_variable)
  SET(${_output_variable})

  # if pkg-config has been found
  IF(PKGCONFIG_EXECUTABLE)

    EXEC_PROGRAM(${PKGCONFIG_EXECUTABLE} ARGS ${_package} --exists RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _pkgconfigDevNull )

    # and if the package of interest also exists for pkg-config, then get the information
    IF(NOT _return_VALUE)

      EXEC_PROGRAM(${PKGCONFIG_EXECUTABLE} ARGS ${_package} --variable ${_var} OUTPUT_VARIABLE ${_output_variable} )

    ENDIF(NOT _return_VALUE)

  ENDIF(PKGCONFIG_EXECUTABLE)

ENDMACRO(PKGCONFIG_GETVAR _package _var _output_variable)

MACRO(dbus_add_activation_service _sources)
    PKGCONFIG_GETVAR(dbus-1 session_bus_services_dir _install_dir)
    FOREACH (_i ${_sources})
        GET_FILENAME_COMPONENT(_service_file ${_i} ABSOLUTE)
        STRING(REGEX REPLACE "\\.service.*$" ".service" _output_file ${_i})
        SET(_target ${CMAKE_CURRENT_BINARY_DIR}/${_output_file})
        CONFIGURE_FILE(${_service_file} ${_target})
        INSTALL(FILES ${_target} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dbus-1/services)
    ENDFOREACH(_i ${ARGN})
ENDMACRO(dbus_add_activation_service _sources)

MACRO(msvc_set_proper_console_handling _target)
    # We do not need a console app when in release
    SET_TARGET_PROPERTIES(${_target} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
    SET_TARGET_PROPERTIES(${_target} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
    SET_TARGET_PROPERTIES(${_target} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    SET_TARGET_PROPERTIES(${_target} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
    SET_TARGET_PROPERTIES(${_target} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    SET_TARGET_PROPERTIES(${_target} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
ENDMACRO(msvc_set_proper_console_handling _target)
