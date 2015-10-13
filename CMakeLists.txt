# Copyright (C) 2015 Florian Franzen
project(neurosuite CXX)
cmake_minimum_required(VERSION 2.8.12)

set(NEUROSUITE_VERSION_MAJOR 2)
set(NEUROSUITE_VERSION_MINOR 0)
set(NEUROSUITE_VERSION_PATCH 0)
set(NEUROSUITE_VERSION
    ${NEUROSUITE_VERSION_MAJOR}.${NEUROSUITE_VERSION_MINOR}.${NEUROSUITE_VERSION_PATCH}
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

# Enable fast QT operators
#add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

if(APPLE)
   # Enable RPATH on OS X (default in CMake >= 3.0)
   set(CMAKE_MACOSX_RPATH ON)
endif(APPLE)

# Configure CMAKE
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#####################
# Find dependencies #
#####################
# Find correct Qt library
set(CMAKE_AUTOMOC ON)
if(WITH_QT4)
    find_package(Qt4 4.8.0 REQUIRED QtGui QtWebKit)

    if(APPLE)
        # Or QStandardPaths port needs these on OS X
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
    @ONLY)

# Create version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_PACKAGE_BUILD_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${NEUROSUITE_VERSION}
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
        DESTINATION ${CMAKE_PACKAGE_FILE_DESTINATION}
)

# Install target
install(EXPORT LibraryTargets
        DESTINATION ${CMAKE_PACKAGE_FILE_DESTINATION}
        FILE "${PROJECT_NAME}Targets.cmake")

######################
# CPack Configuriton #
######################
INCLUDE(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Library for Klusters, Neuroscope and NDManager")
SET(CPACK_PACKAGE_NAME "lib${PROJECT_NAME}")
SET(CPACK_INSTALL_DIRECTORY "${CMAKE_INSTALL_PREFIX}")

SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME_SIMPLIFIED}-${CPACK_PACKAGE_VERSION}-source")
SET(CPACK_PACKAGE_NAME_AND_VERSION "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Source")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_FILE_NAME}")

IF(WIN32)
        SET(CPACK_GENERATOR "NSIS")
        SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME_AND_VERSION}")
        SET(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_NAME_AND_VERSION}")
        SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME_AND_VERSION}")
        SET(CPACK_NSIS_INSTALL_ROOT "neurosuite") # use "$PROGRAMFILES" to get "C:\Program Files" or similar
        SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME_AND_VERSION}")
ELSEIF(APPLE)
        SET(CPACK_GENERATOR "ZIP")
        SET(CPACK_SYSTEM_NAME "OSX")
ELSE()
        SET(CPACK_GENERATOR "TBZ2")

        # Determine distribution and release
        execute_process(COMMAND lsb_release -si OUTPUT_VARIABLE DISTRIBUTION OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(COMMAND lsb_release -sc OUTPUT_VARIABLE RELEASE OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(COMMAND uname -i OUTPUT_VARIABLE ARCHITECTURE)

        # Set package name and type (deb vs rpm)
        if(DISTRIBUTION STREQUAL "Ubuntu")

          # Set Ubuntu-specific information (see http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
          if(ARCHITECTURE MATCHES ".*x86_64.*")
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
          else()
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
          endif()
          set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Michaël Zugaro")
          set(CPACK_DEBIAN_PACKAGE_SECTION "Science")
          set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "nphys-data, klusters, neuroscope, ndmanager")
          set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-network, libqtcore4, libqtgui4, libqtwebkit4")
          set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://klusters.sourceforge.net")
          set(CPACK_GENERATOR "DEB")
          set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-${RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})

        # >>> TEMPLATE CODE FOR ADDITIONAL DEBIAN-LIKE DISTRIBUTIONS <<<

        #elseif(DISTRIBUTION STREQUAL "Ubuntu")
        #
        #  # Set Ubuntu-specific information (see http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
        #  if(ARCHITECTURE MATCHES ".*x86_64.*")
        #    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
        #  else()
        #    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
        #  endif()
        #  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Michaël Zugaro")
        #  set(CPACK_DEBIAN_PACKAGE_SECTION "Science")
        #  set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "nphys-data, klusters, neuroscope, ndmanager")
        #  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-network, libqtcore4, libqtgui4, libqtwebkit4")
        #  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://klusters.sourceforge.net")
        #  set(CPACK_GENERATOR "DEB")
        #  set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-${RELEASE}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})

        elseif(DISTRIBUTION MATCHES "openSUSE.*")

          # Set openSUSE-specific information (see http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
          if(ARCHITECTURE MATCHES ".*x86_64.*")
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
          else()
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "i586")
          endif()
          set(CPACK_PACKAGE_CONTACT "Michaël Zugaro")
          set(CPACK_RPM_PACKAGE_GROUP "Science")
          set(CPACK_RPM_PACKAGE_LICENSE "GPL")
          set(CPACK_RPM_PACKAGE_SUGGESTS "nphys-data, neuroscope, ndmanager")
          set(CPACK_RPM_PACKAGE_REQUIRES "libqt4, libqt4-x11, libQtWebKit4")
          set(CPACK_RPM_PACKAGE_URL "http://klusters.sourceforge.net")
          set(CPACK_GENERATOR "RPM")
          set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE})

        elseif(DISTRIBUTION STREQUAL "Fedora")

          # Set Fedora-specific information (see http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
          execute_process(COMMAND lsb_release -sr OUTPUT_VARIABLE RELEASE OUTPUT_STRIP_TRAILING_WHITESPACE)
          if(ARCHITECTURE MATCHES ".*x86_64.*")
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
          else()
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "i686")
          endif()
          set(CPACK_PACKAGE_CONTACT "Michaël Zugaro")
          set(CPACK_RPM_PACKAGE_GROUP "Science")
          set(CPACK_RPM_PACKAGE_LICENSE "GPL")
          set(CPACK_RPM_PACKAGE_SUGGESTS "nphys-data, neuroscope, ndmanager")
          set(CPACK_RPM_PACKAGE_REQUIRES "qt, qt-mobility, qtwebkit, qt-x11")
          set(CPACK_RPM_PACKAGE_URL "http://klusters.sourceforge.net")
          set(CPACK_GENERATOR "RPM")
          set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.fc${RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE})

        elseif(DISTRIBUTION MATCHES "Scientific")

          # Set Scientific-Linux-specific information (see http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
          if(ARCHITECTURE MATCHES ".*x86_64.*")
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
          else()
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "i386")
          endif()
          set(CPACK_PACKAGE_CONTACT "Rajat Mani Thomas")
          set(CPACK_RPM_PACKAGE_GROUP "Science")
          set(CPACK_RPM_PACKAGE_LICENSE "GPL")
          set(CPACK_RPM_PACKAGE_SUGGESTS "nphys-data, neuroscope, ndmanager")
          set(CPACK_RPM_PACKAGE_REQUIRES "libQtCore, libQtGui, libQtNetwork, libQtWebKit")
          set(CPACK_RPM_PACKAGE_URL "http://klusters.sourceforge.net")
          set(CPACK_GENERATOR "RPM")
          set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE})

        # >>> TEMPLATE CODE FOR ADDITIONAL REDHAT-LIKE DISTRIBUTIONS <<<

        #elseif(DISTRIBUTION MATCHES "openSUSE.*")
        #
        #  # Set openSUSE-specific information (see http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
        #  if(ARCHITECTURE MATCHES ".*x86_64.*")
        #    set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
        #  else()
        #    set(CPACK_RPM_PACKAGE_ARCHITECTURE "i586")
        #  endif()
        #  set(CPACK_PACKAGE_CONTACT "Michaël Zugaro")
        #  set(CPACK_RPM_PACKAGE_GROUP "Science")
        #  set(CPACK_RPM_PACKAGE_LICENSE "GPL")
        #  set(CPACK_RPM_PACKAGE_SUGGESTS "nphys-data, neuroscope, ndmanager")
        #  set(CPACK_RPM_PACKAGE_REQUIRES "libqt4, libqt4-x11, libQtWebKit4")
        #  set(CPACK_RPM_PACKAGE_URL "http://klusters.sourceforge.net")
        #  set(CPACK_GENERATOR "RPM")
        #  set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${RELEASE}.${CPACK_RPM_PACKAGE_ARCHITECTURE})

        endif()
ENDIF()


SET(CPACK_TOPLEVEL_TAG "${CPACK_SYSTEM_NAME}")
FILE(TO_CMAKE_PATH "${CPACK_INSTALL_DIRECTORY}" XPLATFORM_INSTALL_DIR) # to get forward slashes on Windows

SET(CPACK_IGNORE_FILES '.git'  "/[._]obj/" "/moc_" "/[._]o/" "/CMakeFiles/" "/[._]cpp" "/[._]depends" "CMakeCache.txt" "Makefile"
                       "^${XPLATFORM_INSTALL_DIR}/install_manifest.txt"
                       "^${XPLATFORM_INSTALL_DIR}/src/qrc_libklustersshared-icons.cxx"
                       "^${XPLATFORM_INSTALL_DIR}/src/klustersshared_automoc.cpp"
                       "^${XPLATFORM_INSTALL_DIR}/src/cmake_install.cmake"
                       "^${XPLATFORM_INSTALL_DIR}/src/gui/libklustersshared-icons.qrc.depends"
                       "^${XPLATFORM_INSTALL_DIR}/src/qpageview_p.moc"
                       "^${XPLATFORM_INSTALL_DIR}/src/CPackSourceConfig.cmake"
                       "^${XPLATFORM_INSTALL_DIR}/CPackSourceConfig.cmake"
                       "^${XPLATFORM_INSTALL_DIR}/CMakeTmp/check_qt_visibility.cpp"
                       "^${XPLATFORM_INSTALL_DIR}/CPackConfig.cmake"
                       "^${XPLATFORM_INSTALL_DIR}/cmake_install.cmake")


include(CPack)
