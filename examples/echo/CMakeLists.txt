set(TARGET "sne_echo")

if(SNE_BUILD_SHARED)
    # On WIN32, a shared library require proper dllexport/dllimport declarations.
    # With MinGW the default is to export all, but with Visual Studio the default is export none.
    # The header files will do this correctly with the following defines
    add_definitions( -DANYRPC_DLL )
endif ()

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h)

add_executable(${TARGET} ${sources})

add_precompiled_header(${TARGET} EchoTestPCH.h)

target_link_libraries(${TARGET}
    ${SNE_CLIENT}
    ${SNE_SERVER}
    ${Boost_LIBRARIES}
)

install(TARGETS ${TARGET}
         RUNTIME DESTINATION ${SNE_BIN_INSTALL_DIR}
         LIBRARY DESTINATION ${SNE_LIB_INSTALL_DIR}
         ARCHIVE DESTINATION ${SNE_LIB_INSTALL_DIR})
