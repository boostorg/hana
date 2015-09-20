# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup project
##############################################################################
cmake_minimum_required(VERSION 3.0)
project(Boost.Hana CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
enable_testing()

# Perform checks to make sure we support the current compiler
include(CheckCxxCompilerSupport)


##############################################################################
# Setup CMake options
##############################################################################
option(BOOST_HANA_ENABLE_WERROR   "Fail and stop if a warning is triggered." OFF)
option(BOOST_HANA_ENABLE_MEMCHECK "Run the unit tests and examples under Valgrind if it is found." OFF)
option(BOOST_HANA_ENABLE_CONCEPT_CHECKS "Enable concept checking in the interface methods." ON)

option(BOOST_HANA_ENABLE_STRING_UDL
"Enable the GNU extension allowing the special string literal operator\
 template, which enables the _s suffix for creating compile-time strings." ON)

option(BOOST_HANA_ENABLE_EXCEPTIONS
"Build with exceptions enabled. Note that Hana does not make use of exceptions,\
 but this switch can be disabled when building the tests to assess that it is\
 really the case." ON)


##############################################################################
# Setup compiler flags (more can be set on a per-target basis or in subdirectories)
##############################################################################
include(CheckCXXCompilerFlag)
macro(boost_hana_append_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

# Compiler flags controlled by CMake options above
if (BOOST_HANA_ENABLE_WERROR)
    boost_hana_append_flag(BOOST_HANA_HAS_WERROR -Werror)
    boost_hana_append_flag(BOOST_HANA_HAS_WX -WX)
endif()

if (NOT BOOST_HANA_ENABLE_CONCEPT_CHECKS)
    add_definitions(-DBOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS)
endif()

if (BOOST_HANA_ENABLE_STRING_UDL)
    add_definitions(-DBOOST_HANA_CONFIG_ENABLE_STRING_UDL)
    # GCC pretends to have the flag, but produces a "unrecognized command line option"
    # warning when we use it.
    if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        boost_hana_append_flag(BOOST_HANA_HAS_WNO_GNU_STRING_UDL
                               -Wno-gnu-string-literal-operator-template)
    endif()
endif()

if (NOT BOOST_HANA_ENABLE_EXCEPTIONS)
    boost_hana_append_flag(BOOST_HANA_HAS_FNO_EXCEPTIONS -fno-exceptions)
endif()

# Other compiler flags
boost_hana_append_flag(BOOST_HANA_HAS_FTEMPLATE_BACKTRACE_LIMIT  -ftemplate-backtrace-limit=0)
boost_hana_append_flag(BOOST_HANA_HAS_PEDANTIC                   -pedantic)
boost_hana_append_flag(BOOST_HANA_HAS_STDCXX1Y                   -std=c++1y)
boost_hana_append_flag(BOOST_HANA_HAS_W                          -W)
boost_hana_append_flag(BOOST_HANA_HAS_WALL                       -Wall)
boost_hana_append_flag(BOOST_HANA_HAS_WEXTRA                     -Wextra)
boost_hana_append_flag(BOOST_HANA_HAS_WNO_UNUSED_LOCAL_TYPEDEFS  -Wno-unused-local-typedefs)
boost_hana_append_flag(BOOST_HANA_HAS_WWRITE_STRINGS             -Wwrite-strings)


##############################################################################
# Search for packages.
#
# Behavior when the package is found or not is customized at the
# point where the package is required.
##############################################################################
find_package(Boost)
find_package(Doxygen)
find_package(Git)
find_package(Meta)
find_package(MPL11)
find_package(PythonInterp 2.7)
find_package(Ruby 2.1)

if (BOOST_HANA_ENABLE_MEMCHECK)
    find_package(Valgrind REQUIRED)
endif()

if (NOT ${Boost_FOUND})
    message(WARNING
        "The Boost library headers were not found; targets depending "
        "on Boost won't be available.")
endif()

if (GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        OUTPUT_VARIABLE BOOST_HANA_CURRENT_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()


##############################################################################
# Setup the URL to the official repository on GitHub.
#
# If there is a GITHUB_TOKEN environment variable, we assume this token allows
# us to push to the GitHub repository and we use that in the URL. This token
# will give us push access for updating the 'gh-pages' branch and doing other
# useful things e.g. from Travis. Otherwise, if there's no such environment
# variable, the URL just refers to the GitHub repository without authentication.
# In this case, we'll be asked for our username and password when/if we try to
# push to the repository.
##############################################################################
if (DEFINED ENV{GITHUB_TOKEN})
    set(BOOST_HANA_ORIGIN_URL "https://ldionne:$ENV{GITHUB_TOKEN}@github.com/ldionne/hana")
else()
    set(BOOST_HANA_ORIGIN_URL "https://github.com/ldionne/hana")
endif()


##############################################################################
# Setup custom functions to ease the creation of targets
##############################################################################
#   boost_hana_target_name_for(<output variable> <source file> [ext])
#
# Return the target name associated to a source file. If the path of the
# source file relative from the root of Hana is `path/to/source/file.ext`,
# the target name associated to it will be `path.to.source.file`.
#
# The extension of the file should be specified as a last argument. If no
# extension is specified, the `.cpp` extension is assumed.
function(boost_hana_target_name_for out file)
    if (NOT ARGV2)
        set(_extension ".cpp")
    else()
        set(_extension "${ARGV2}")
    endif()

    file(RELATIVE_PATH _relative ${Boost.Hana_SOURCE_DIR} ${file})
    string(REPLACE "${_extension}" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()

#   boost_hana_add_test(<name> <command> [<arg>...])
#
# Creates a test called `name`, which runs the given `command` with the given
# `arg`uments. However, if `BOOST_HANA_ENABLE_MEMCHECK` is set to `ON`, the
# test will run the provided command under the memory checker.
function(boost_hana_add_test name)
    if (BOOST_HANA_ENABLE_MEMCHECK)
        add_test(${name} ${Valgrind_EXECUTABLE} --leak-check=full --error-exitcode=1 ${ARGN})
    else()
        add_test(${name} ${ARGN})
    endif()
endfunction()

#   boost_hana_list_remove_glob(<list> <GLOB|GLOB_RECURSE> [globbing expressions]...)
#
# Generates a list of files matching the given glob expressions, and remove
# the matched elements from the given <list>.
macro(boost_hana_list_remove_glob list glob)
    file(${glob} _bhlrg10321023_avoid_macro_clash_matches ${ARGN})
    list(REMOVE_ITEM ${list} ${_bhlrg10321023_avoid_macro_clash_matches})
endmacro()


##############################################################################
# Setup the `check` target to build and then run all the tests and examples.
##############################################################################
add_custom_target(check ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Build and then run all the tests and examples.")

##############################################################################
# Setup subdirectories
##############################################################################
add_subdirectory(benchmark)
add_subdirectory(doc)
add_subdirectory(example)
add_subdirectory(experimental)
add_subdirectory(test)
