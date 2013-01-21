
cp plexydesk-bb10.xml ${CMAKE_INSTALL_PREFIX}
cd ${CMAKE_INSTALL_PREFIX}
blackberry-nativepackager -devMode -debugToken bb10.bar -package ${CMAKE_CURRENT_BINARY_DIR}/plexydesk-${APPLICATION_MAIN_VERSION}.bar plexydesk-bb10.xml


