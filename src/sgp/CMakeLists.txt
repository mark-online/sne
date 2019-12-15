set(TARGET ${SNE_SGP})

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

include_directories(
  ${SNE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

if(SNE_BUILD_SHARED)
    add_definitions(-DSNE_EXPORT)
endif()

add_library(${TARGET} ${SNE_LIB_TYPE}
  ${sne_headers_common}
  ${sne_headers_core}
  ${sne_headers_base}
  ${sne_headers_sgp}
  ${sources}
)

add_precompiled_header(${TARGET} SgpPCH.h)

target_link_libraries(${TARGET} ${SNE_SRPC} ${SNE_SECURITY})

install(TARGETS ${TARGET}
         RUNTIME DESTINATION ${SNE_BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${SNE_LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${SNE_LIB_INSTALL_DIR})
