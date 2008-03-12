project(blitz_lib) 
include(CheckCXXSourceCompiles)
include(CheckIncludeFile)

if (APPLE)
 set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})
 set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif (APPLE)

include_directories(
${CMAKE_SOURCE_DIR}  
${blitz_lib_BINARY_DIR} 
${QT_INCLUDE_DIR}
${QT_QTCORE_INCLUDE_DIR}  
${QT_QTGUI_INCLUDE_DIR}
)

CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)

if(NOT MSVC)
  CHECK_CXX_SOURCE_COMPILES(" int main() { __asm__(\"pxor %mm0, %mm0\") ; }" HAVE_MMX)
  CHECK_CXX_SOURCE_COMPILES(" int main() { __asm__(\"xorps %xmm0, %xmm0\"); }" HAVE_SSE)
  CHECK_CXX_SOURCE_COMPILES(" int main() { __asm__(\"xorpd %xmm0, %xmm0\"); }" HAVE_SSE2)
  CHECK_CXX_SOURCE_COMPILES(" int main() { __asm__(\"pi2fd %mm0, %mm0\"); }" HAVE_3DNOW)
  CHECK_CXX_SOURCE_COMPILES(" int main() { __asm__(\"mtspr 256, %0; vand %%v0, %%v0, %%v0\" : : \"r\"(-1) ); }" HAVE_ALTIVEC)
endif(NOT MSVC)

SET(BLITZ_LIB_MAJOR_VERSION "4")
SET(BLITZ_LIB_MINOR_VERSION "0")
SET(BLITZ_LIB_PATCH_VERSION "0")

SET(BLITZ_LIB_VERSION_STRING "${BLITZ_LIB_MAJOR_VERSION}.${BLITZ_LIB_MINOR_VERSION}.${BLITZ_LIB_PATCH_VERSION}")

set(blitz_LIB_SRCS blitz.cpp colors.cpp convolve.cpp histogram.cpp
blitzcpu.cpp scale.cpp scalefilter.cpp gradient.cpp)

if(HAVE_MMX)
    if(NOT APPLE AND NOT WIN32)
        set( blitz_LIB_SRCS ${blitz_LIB_SRCS} asm_scale.S )
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/asm_scale.o
            COMMAND gcc -c ${CMAKE_CURRENT_SOURCE_DIR}/asm_scale.S
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/asm_scale.S )
        set( blitz_LIB_EXTRA_SRCS ${CMAKE_CURRENT_BINARY_DIR}/asm_scale.o )
        set(HAVE_EXTERNAL_ASM TRUE)
    endif(NOT APPLE AND NOT WIN32)
endif(HAVE_MMX)
configure_file (config-processor.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-processor.h )
configure_file (config-externalasm.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-externalasm.h )

add_library(qimageblitz SHARED ${blitz_LIB_SRCS} ${blitz_LIB_EXTRA_SRCS})

target_link_libraries(qimageblitz ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
if (NOT CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SYSTEM_NAME MATCHES SunOS)
    target_link_libraries(qimageblitz -lm -lsunmath)
endif (NOT CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SYSTEM_NAME MATCHES SunOS)

set_target_properties(qimageblitz PROPERTIES
                            VERSION ${BLITZ_LIB_MAJOR_VERSION}.${BLITZ_LIB_MINOR_VERSION}.${BLITZ_LIB_PATCH_VERSION}
                            SOVERSION ${BLITZ_LIB_MAJOR_VERSION}
                            DEFINE_SYMBOL MAKE_QIMAGEBLITZ_LIB
)
install( TARGETS qimageblitz  
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} 
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} 
)  
install( FILES qimageblitz.h DESTINATION ${INCLUDE_INSTALL_DIR} ) 
install( FILES qimageblitz_export.h DESTINATION ${INCLUDE_INSTALL_DIR} )
install( FILES blitzcpu.h DESTINATION ${INCLUDE_INSTALL_DIR} )

if(NOT WIN32)
  configure_file("qimageblitz.pc.cmake" "${CMAKE_CURRENT_BINARY_DIR}/qimageblitz.pc"
  @ONLY) 

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qimageblitz.pc DESTINATION 
  ${LIB_INSTALL_DIR}/pkgconfig)  
endif(NOT WIN32)


