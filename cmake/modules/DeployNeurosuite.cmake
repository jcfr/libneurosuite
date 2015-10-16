# - Functions to help deploy Neurosuite apps.
#
# The following functions are provided by this module:
#   install_neurosuite_dependencies
#
# Requires CMake 2.6 or greater because it uses function and
# PARENT_SCOPE. Also depends on DeployQt4.cmake, DeployQt5.cmake and
# InstallRequiredSystemLibraries.cmake
#
#
# INSTALL_NEUROSUITE_DEPENDENCIES(<executable>)
#   Installs msvc, qt, neurosuite and cbsdk libs for <executable>.
#   To determine what to install, the following variables are used:
#       WITH_QT4      - True if qt4 is used (else qt5)
#       WITH_CEREBUS  - True if cbsdk is needed/used
#

function(install_neurosuite_dependencies _EXECUTABLE)
    # Install MSVC runtime dlls if needed
    if(MSVC)
        include(InstallRequiredSystemLibraries)
    endif()

    # Fix unset QT_LIBRARY_DIR if possible
    if(NOT QT_LIBRARY_DIR)
        if(NOT WITH_QT4)
            if(TARGET Qt5::Core)
                get_property(LOCATION TARGET Qt5::Core PROPERTY LOCATION)
                get_filename_component(QT_LIBRARY_DIR ${LOCATION} PATH)
            else()
                message(WARNING "Qt5::Core not defined, will not be able to determine QT_LIBRARY_DIR automatically.")
            endif()
        endif()
    endif()

    # Determine dependencies search path
    if(MINGW)
        find_path(MINGW_DIR libstdc++-6.dll)
        list(APPEND DIRS "${MINGW_DIR}")
    endif()

    get_property(LOCATION TARGET neurosuite PROPERTY LOCATION)
    get_filename_component(NEUROSUITE_DIR ${LOCATION} PATH)
    list(APPEND DIRS "${NEUROSUITE_DIR}")

    if(WITH_CEREBUS)
        get_property(LOCATION TARGET cbsdk PROPERTY LOCATION)
        get_filename_component(CBSDK_DIR ${LOCATION} PATH)
        list(APPEND DIRS "${CBSDK_DIR}")
    endif()

    # Determine plugins
    if(APPLE AND WITH_QT4)
        list(APPEND PLUGINS qcocoa)
    endif()

    # Use DeployQt4/5 to install qt and other libraries
    if(WITH_QT4)
        include(DeployQt4)
        install_qt4_executable("${_EXECUTABLE}" "${PLUGINS}" "" "${DIRS}")
    else()
        include(DeployQt5)
        install_qt5_executable("${_EXECUTABLE}" "${PLUGINS}" "" "${DIRS}")
    endif()
endfunction()
