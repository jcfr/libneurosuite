project(libklustersshared)
cmake_minimum_required(VERSION 2.8.6)
set (QT_MIN_VERSION "4.7.0")
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} )

# tell cmake where to search for Qt headers:
include_directories(${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
set(EXEC_INSTALL_PREFIX  ${CMAKE_INSTALL_PREFIX}       CACHE PATH  "Base directory for executables and libraries")
#
## the following are directories where stuff will be installed to
set(BIN_INSTALL_DIR          "${EXEC_INSTALL_PREFIX}/bin" CACHE PATH "The binary install dir (default prefix/bin)")
set(LIB_INSTALL_DIR          "${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX}"     CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is /lib${LIB_SUFFIX})")
set(INCLUDE_INSTALL_DIR          "${EXEC_INSTALL_PREFIX}/include/klustersshared"     CACHE PATH "The subdirectory relative to the install prefix where includes will be installed ")

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

configure_file(LibKlustersSharedConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibKlustersSharedConfig.cmake @ONLY)

add_subdirectory(src)

# install the file with the exported targets
install(EXPORT LibKlustersSharedLibraryTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKlustersShared
        FILE LibKlustersSharedTargetsWithPrefix.cmake
        )


install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/LibKlustersSharedConfig.cmake
  DESTINATION ${LIB_INSTALL_DIR}/cmake/LibKlustersShared)


