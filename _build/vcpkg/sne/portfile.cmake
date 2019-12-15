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
set(SOURCE_PATH ${CURRENT_BUILDTREES_DIR}/src/sne-0.0.1)
vcpkg_download_distfile(ARCHIVE
    URLS "http://localhost:8080/sne-0.0.1.zip"
    FILENAME "sne-0.0.1.zip"
    SHA512 284be7327c87b9c2b433aca49efa8a4a25e8e949f96a17a06a1794e740bf07f8d1fd9a72d6a5008f6a7f8554a7c4a487d1baa643cabe7442872b0f449d388c24
)
vcpkg_extract_source_archive(${ARCHIVE})

set(OPTIONS)

if(VCPKG_LIBRARY_LINKAGE STREQUAL dynamic)
    list(APPEND OPTIONS -DSNE_BUILD_SHARED=ON)
endif()

if(VCPKG_CRT_LINKAGE STREQUAL static)
  list(APPEND OPTIONS -DSNE_MSVC_STATIC_RUNTIME=ON)
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA # Disable this option if project cannot be built with Ninja
    OPTIONS ${OPTIONS}
)

file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

vcpkg_install_cmake()

# Include files should not be duplicated into the /debug/include directory.
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

vcpkg_copy_pdbs()

# Handle copyright
file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/sne RENAME copyright)

# Post-build test for cmake libraries
# vcpkg_test_cmake(PACKAGE_NAME sne)
