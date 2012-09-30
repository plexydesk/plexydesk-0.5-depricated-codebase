SET (jsoncppsrc
     jsoncpp.cpp
    )

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY (jsoncpp SHARED 
             ${jsoncppsrc} )

#SET_TARGET_PROPERTIES(jsoncpp PROPERTIES
#                      COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})


INSTALL(TARGETS jsoncpp DESTINATION ${CMAKE_INSTALL_LIBDIR})
