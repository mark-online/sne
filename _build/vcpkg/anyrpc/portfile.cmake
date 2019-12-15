# Common Ambient Variables:
#   CURRENT_BUILDTREES_DIR    = ${VCPKG_ROOT_DIR}\buildtrees\${PORT}
#   CURRENT_PACKAGES_DIR      = ${VCPKG_ROOT_DIR}\packages\${PORT}_${TARGET_TRIPLET}
#   CURRENT_PORT_DIR          = ${VCPKG_ROOT_DIR}\ports\${PORT}
#   PORT                      = current port name (zlib, etc)
#   TARGET_TRIPLET            = current triplet (x86-windows, x64-windows-static, etc)
#   VCPKG_CRT_LINKAGE         = C runtime linkage type (static, dynamic)
#   VCPKG_LIBRARY_LINKAGE     = target library linkage type (static, dynamic)
#   VCPKG_ROOT_DIR            = <C:\path\to\current\vcpkg>
#   VCPKG_TARGET_ARCHITECTURE = target architecture (x64, x86, arm)
#

include(vcpkg_common_functions)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO sgieseking/anyrpc
    #REF v1.0.1
    REF master
    SHA512 059396ef9d893af37811d50ff6d67885f8222d962fd33964b2ea2248e42f14ce1b64fed26833b90368ca3ff7f6accbe69c7d1b34b1ff7111ff17e1c49ea60f29
    HEAD_REF master
)

set(OPTIONS)

if(VCPKG_LIBRARY_LINKAGE STREQUAL static)
    list(APPEND OPTIONS -DANYRPC_LIB_BUILD_SHARED=OFF)
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA # Disable this option if project cannot be built with Ninja
    OPTIONS
        ${OPTIONS}
        -DBUILD_EXAMPLES=OFF
        -DBUILD_TESTS=OFF
        -DBUILD_WITH_LOG4CPLUS=OFF
        -DBUILD_PROTOCOL_XML=OFF
        -DBUILD_PROTOCOL_MESSAGEPACK=OFF
)

vcpkg_install_cmake()

# Include files should not be duplicated into the /debug/include directory.
# If this cannot be disabled in the project cmake, use
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

# Handle copyright
file(INSTALL ${SOURCE_PATH}/license DESTINATION ${CURRENT_PACKAGES_DIR}/share/anyrpc RENAME copyright)

vcpkg_copy_pdbs()

# Post-build test for cmake libraries
# vcpkg_test_cmake(PACKAGE_NAME anyrpc)
