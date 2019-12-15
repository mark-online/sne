set(TARGET ${SNE_CORE})

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
  ${sources}
)

add_precompiled_header(${TARGET} CorePCH.h)

target_link_libraries(${TARGET})

install(TARGETS ${TARGET}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib
         COMPONENT library)
