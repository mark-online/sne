set(TARGET "${SNE_SGP}.test")

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${TARGET}
  ${sources}
)

add_precompiled_header(${TARGET} SgpTestPCH.h)

target_link_libraries(${TARGET} ${SNE_SGP} GTest::GTest)

add_test(AllTests ${TARGET})

install(TARGETS ${TARGET}
         RUNTIME DESTINATION ${SNE_BIN_INSTALL_DIR})
