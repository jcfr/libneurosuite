# Copyright (C) 2015 Florian Franzen
project(neurosuite CXX)
cmake_minimum_required(VERSION 2.8.12)

set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

# Options
option(WITH_QT4
       "Enable to build against Qt4 (default: Qt5)"
       OFF)

option(WITH_TEST
       "Build scrollarea test binary"
       OFF)

##############################
# Configure build environment #
###############################

# Enable exports for library build
add_definitions(-DNEUROSUITE_BUILD_NEUROSUITE_LIB)

if(APPLE)
   # Enable RPATH on OS X (default in CMake >= 3.0)
   set(CMAKE_MACOSX_RPATH ON)
endif(APPLE)

# Configure CMAKE
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Detect 64bit build
set(PROJECT_IS_64BIT CMAKE_SIZEOF_VOID_P EQUAL 8)

#####################
# Find dependencies #
#####################
# Configure Qt functionality
set(CMAKE_AUTOMOC ON)
add_definitions(-DQT_USE_QSTRINGBUILDER)

# Find correct Qt library
if(WITH_QT4)
    find_package(Qt4 4.8.0 REQUIRED QtGui QtWebKit)

    if(APPLE)
        # The QStandardPaths port needs these on OS X
        find_library(CF_LIBRARIES NAMES CoreFoundation)
        find_library(CS_LIBRARIES NAMES CoreServices)
    endif()
else()
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5PrintSupport REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
endif()

if (MSVC)
   # Qt disables the native wchar_t type, do it too to avoid linking issues
   #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Zc:wchar_t-" )
endif()

########################
# Include subdirectory #
########################
# Build main library
add_subdirectory(src)

# Build test binary
if(WITH_TEST)
    add_subdirectory(test)
endif()

##############################
# Create CMake package files #
##############################
set(CMAKE_PACKAGE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/cmake")

# Create targets file (disabled, does not work in cmake < 3.0.0)
# export(EXPORT LibraryTargets
#        FILE "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}Targets.cmake"
# )

# Create config file
configure_file(cmake/PackageConfig.cmake.in
    "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}Config.cmake"
    @ONLY
)

configure_file(cmake/NeurosuiteHelpers.cmake
    "${CMAKE_PACKAGE_BUILD_DIR}/NeurosuiteHelpers.cmake"
    COPYONLY
)
configure_file(cmake/DeployQt5.cmake
    "${CMAKE_PACKAGE_BUILD_DIR}/DeployQt5.cmake"
    COPYONLY
)

# Create version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

###############################
# Install CMake package files #
###############################
if(WIN32)
    set(CMAKE_PACKAGE_FILE_DESTINATION "CMake")
else()
    set(CMAKE_PACKAGE_FILE_DESTINATION "lib/cmake/${PROJECT_NAME}")
endif()

# Install config and version
install(FILES "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}Config.cmake"
              "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
              "${CMAKE_PACKAGE_BUILD_DIR}/NeurosuiteHelpers.cmake"
              "${CMAKE_PACKAGE_BUILD_DIR}/DeployQt5.cmake"
        DESTINATION ${CMAKE_PACKAGE_FILE_DESTINATION}
)

# Install target
install(EXPORT LibraryTargets
        DESTINATION ${CMAKE_PACKAGE_FILE_DESTINATION}
        FILE "${PROJECT_NAME}Targets.cmake")

######################
# CPack Configuriton #
######################
set(CPACK_PACKAGE_NAME "lib${PROJECT_NAME}")
set(CPACK_PACKAGE_CONTACT "Florian Franzen <FlorianFranzen@gmail.com>")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library for shared functionality between Klusters, Neuroscope and NDManager")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    if(PROJECT_IS_64BIT)
        set(CPACK_SYSTEM_NAME "win64")
    else()
        set(CPACK_SYSTEM_NAME "win32")
    endif()
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_SYSTEM_NAME "osx")
else()
    set(CPACK_GENERATOR "DEB")

    if(PROJECT_IS_64BIT)
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    else()
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
    endif()
    set(CPACK_SYSTEM_NAME "${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    if(WITH_QT4)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqtgui4 (>= 4.8.0), libqtwebkit4 (>= 4.8.0)")
    else()
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5, libqt5printsupport5, libqt5webkit5")
    endif()

    set(CPACK_DEBIAN_PACKAGE_SECTION "Science")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://neurosuite.github.io/")
endif()

if(WITH_QT4)
    set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}-qt4")
else()
    set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}-qt5")
endif()

include(CPack)
