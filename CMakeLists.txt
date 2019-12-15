cmake_minimum_required(VERSION 3.5)

project(SNE LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(PrecompiledHeader)

# = Build options
option(SNE_BUILD_EXAMPLES "Build example projects." OFF)
option(SNE_BUILD_TESTS "Build tests projects." OFF)
option(SNE_BUILD_SHARED "Build sne as a shared library." OFF)
option(SNE_MSVC_STATIC_RUNTIME "MSVC only: build with /MT instead of /MD" OFF)

if(MSVC)
    if (SNE_MSVC_STATIC_RUNTIME)
        set(variables
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        message(STATUS "MSVC -> forcing use of statically-linked runtime.")
        foreach(variable ${variables})
            if(${variable} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
            endif()
        endforeach()
    endif()
else()
	add_compile_options(-Wall)
	# https://stackoverflow.com/a/9832941/5642066
	add_compile_options(-Wno-unused-function)
endif()

# Set whether to build as shared or static library
if(SNE_BUILD_SHARED)
    set(SNE_LIB_TYPE SHARED)
    add_definitions(-DSNE_DLL)
else()
    set(SNE_LIB_TYPE STATIC)
endif()

# = Multi-thread
find_package(Threads REQUIRED)

# = C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# = Boost (https://cmake.org/cmake/help/latest/module/FindBoost.html)
if (SNE_BUILD_SHARED)
    add_definitions(-DBOOST_LOG_DYN_LINK)
else()
    set(Boost_USE_STATIC_LIBS ON) # only find static libs
endif()
set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS system thread filesystem log program_options)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif()

# = Crypto++
find_package(cryptopp CONFIG REQUIRED)

# = anyrpc
option(BUILD_EXAMPLES "Build AnyRPC examples." OFF)
option(BUILD_TESTS "Build AnyRPC unit tests." OFF)
option(BUILD_WITH_LOG4CPLUS "Build log4cplus." OFF)
option(BUILD_PROTOCOL_XML "Build with Xml procotol included." OFF)
option(BUILD_PROTOCOL_MESSAGEPACK "Build with MessgePack protocol included." OFF)
option(ANYRPC_LIB_BUILD_SHARED "Build anyrpc as a shared library." OFF)
include_directories(${CMAKE_SOURCE_DIR}/vendorsrc/anyrpc/include)
add_subdirectory(vendorsrc/anyrpc)

# = StackWalker
set(SNE_STACKWALKER_DIR ${CMAKE_SOURCE_DIR}/vendorsrc/StackWalker)

# = Variables
set(SNE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SNE_LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

set(SNE_CORE "sne.core")
set(SNE_BASE "sne.base")
set(SNE_SECURITY "sne.security")
set(SNE_SRPC "sne.srpc")
set(SNE_SGP "sne.sgp")
set(SNE_CLIENT "sne.client")
set(SNE_SERVER "sne.server")

# = INSTALL
set(SNE_BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE STRING "Directory where binaries will install")
set(SNE_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Directory where libraries will install")
set(SNE_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include/sne" CACHE STRING "Directory where headers will install")

# = include
file(GLOB_RECURSE sne_headers_common ${SNE_INCLUDE_DIR}/sne/common/*.h ${SNE_INCLUDE_DIR}/sne/Common.h)
file(GLOB_RECURSE sne_headers_core ${SNE_INCLUDE_DIR}/sne/core/*.h)
file(GLOB_RECURSE sne_headers_base ${SNE_INCLUDE_DIR}/sne/base/*.h)
file(GLOB_RECURSE sne_headers_security ${SNE_INCLUDE_DIR}/sne/security/*.h)
file(GLOB_RECURSE sne_headers_srpc ${SNE_INCLUDE_DIR}/sne/srpc/*.h)
file(GLOB_RECURSE sne_headers_sgp ${SNE_INCLUDE_DIR}/sne/sgp/*.h)
file(GLOB_RECURSE sne_headers_server ${SNE_INCLUDE_DIR}/sne/server/*.h)
file(GLOB_RECURSE sne_headers_client ${SNE_INCLUDE_DIR}/sne/client/*.h)

# = directories
include_directories(${SNE_INCLUDE_DIR})
link_directories(${SNE_LIB_DIR})

# = src
add_subdirectory(src)

# = examples
if(SNE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# = tests
if (SNE_BUILD_TESTS)
    enable_testing()
    find_package(GTest MODULE REQUIRED)

    add_subdirectory(test)
endif()

# = install
install(DIRECTORY include/sne
    DESTINATION "${INCLUDE_INSTALL_DIR}"
    COMPONENT dev)
