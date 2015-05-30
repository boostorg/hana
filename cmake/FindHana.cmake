# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module finds the Hana library. It sets the following CMake
# variables:
#
# BOOST_HANA_FOUND
#   Set to 1 when Hana is found, set to 0 otherwise.
#
# BOOST_HANA_INCLUDE_DIR
#   If Hana is found, this is set to the path of Hana's include/ directory.
#   If Hana is not found, this is not set.
#
#
# The following variables can be used to customize the behavior of the module:
#
# BOOST_HANA_INCLUDE_DIR
#   The path to Hana's include/ directory. When set, this will be used as-is.
#
# BOOST_HANA_CLONE_DIR
#   If Hana can't be found and this is set to something, Hana will be cloned
#   into that directory as an external Git project. The build directory of
#   that external project will be `${CMAKE_CURRENT_BINARY_DIR}/hana`.
#   A suggestion is to set `BOOST_HANA_CLONE_DIR` to `project_root/ext/hana`,
#   and to Git-ignore the whole `ext/` subdirectory, so external dependencies
#   are not tracked by the current project.
#
# BOOST_HANA_ENABLE_TESTS
#   If Hana is cloned as an external project and this is set to true, the
#   tests will be run whenever Hana is updated. By default, this is set to
#   false because the tests tend to be quite long.

if (NOT EXISTS "${BOOST_HANA_INCLUDE_DIR}")
    find_path(BOOST_HANA_INCLUDE_DIR NAMES boost/hana.hpp
              DOC "Include directory for the Hana library")
endif()

if (NOT EXISTS "${BOOST_HANA_INCLUDE_DIR}" AND DEFINED BOOST_HANA_CLONE_DIR)
    set(_build_dir "${CMAKE_CURRENT_BINARY_DIR}/hana")

    if (DEFINED BOOST_HANA_ENABLE_TESTS)
        set(_test_cmd ${CMAKE_COMMAND} --build ${_build_dir} --target check)
    else()
        set(_test_cmd "")
    endif()

    include(ExternalProject)
    ExternalProject_Add(Hana
        PREFIX ${_build_dir}
        STAMP_DIR ${_build_dir}/_stamps
        TMP_DIR ${_build_dir}/_tmp

        # Since we don't have any files to download, we set the DOWNLOAD_DIR
        # to TMP_DIR to avoid creating a useless empty directory.
        DOWNLOAD_DIR ${_build_dir}/_tmp

        # Download step
        GIT_REPOSITORY https://github.com/ldionne/hana
        GIT_TAG master
        TIMEOUT 20

        # Configure step
        SOURCE_DIR "${BOOST_HANA_CLONE_DIR}"
        CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}

        # Build step (nothing to build because Hana is header only)
        BINARY_DIR "${_build_dir}"
        BUILD_COMMAND ""

        # Install step (nothing to be done)
        INSTALL_COMMAND ""

        # Test step
        TEST_COMMAND ${_test_cmd}
    )

    set(BOOST_HANA_INCLUDE_DIR "${BOOST_HANA_CLONE_DIR}/include")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BOOST_HANA DEFAULT_MSG BOOST_HANA_INCLUDE_DIR)
