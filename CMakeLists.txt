# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup project
##############################################################################
project(Boost.Hana CXX)
cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


##############################################################################
# Setup CMake options
##############################################################################
include(CMakeDependentOption)
option(BOOST_HANA_ENABLE_PEDANTIC "Compile with pedantic enabled." ON)
option(BOOST_HANA_ENABLE_WERROR   "Fail and stop if a warning is triggered." OFF)
option(BOOST_HANA_ENABLE_LIBCXX   "Use the -stdlib=libc++ if the compiler supports it." ON)


##############################################################################
# Setup compiler flags (more can be set on a per-target basis)
##############################################################################
include(CheckCXXCompilerFlag)
macro(boost_hana_append_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

boost_hana_append_flag(BOOST_HANA_HAS_W                          -W)
boost_hana_append_flag(BOOST_HANA_HAS_WALL                       -Wall)
boost_hana_append_flag(BOOST_HANA_HAS_WEXTRA                     -Wextra)
boost_hana_append_flag(BOOST_HANA_HAS_WNO_LONG_LONG              -Wno-long-long)
boost_hana_append_flag(BOOST_HANA_HAS_WNO_UNUSED_LOCAL_TYPEDEFS  -Wno-unused-local-typedefs)
boost_hana_append_flag(BOOST_HANA_HAS_WWRITE_STRINGS             -Wwrite-strings)
boost_hana_append_flag(BOOST_HANA_HAS_FTEMPLATE_BACKTRACE_LIMIT  -ftemplate-backtrace-limit=0)
boost_hana_append_flag(BOOST_HANA_HAS_FDEFINE_SIZED_DEALLOCATION "-Xclang -fdefine-sized-deallocation")
boost_hana_append_flag(BOOST_HANA_HAS_STDCXX1Y                   -std=c++1y)

if (BOOST_HANA_ENABLE_WERROR)
    boost_hana_append_flag(BOOST_HANA_HAS_WERROR -Werror)
    boost_hana_append_flag(BOOST_HANA_HAS_WX -WX)
endif()

if (BOOST_HANA_ENABLE_PEDANTIC)
    boost_hana_append_flag(BOOST_HANA_HAS_PEDANTIC -pedantic)
endif()

if (BOOST_HANA_ENABLE_LIBCXX)
    boost_hana_append_flag(BOOST_HANA_HAS_STDLIB_LIBCXX -stdlib=libc++)
endif()


##############################################################################
# Search for packages.
#
# Behavior when the package is found or not is customized at the
# point where the package is required.
##############################################################################
find_package(Boost)
find_package(Doxygen)
find_package(Git)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(WARNING
        "The Boost library headers were not found; targets depending "
        "on Boost won't be available.")
endif()


##############################################################################
# Setup subdirectories
##############################################################################
add_custom_target(compile.all
    COMMENT "Build all the unit tests and examples.")

add_custom_target(run.all ALL
    COMMENT "Build and then run all the unit tests and examples.")

#   boost_hana_target_name_for(<output variable> <source file>)
#
# Return the target name associated to a source file. If the path of the
# source file relative from the root of Hana is `path/to/source/file.cpp`,
# the target name associated to it is `path.to.source.file`.
function(boost_hana_target_name_for out file)
    file(RELATIVE_PATH _relative ${CMAKE_SOURCE_DIR} ${file})
    string(REGEX REPLACE "\\.cpp$" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()

if (DEFINED LIBCXX_ROOT)
    find_library(libcxx c++ HINTS ${LIBCXX_ROOT}/lib)
endif()

#   boost_hana_add_executable(<name> ...)
#
# Calls `add_executable` with exactly the same arguments that were received,
# but also links the executable with the custom libc++ installation
# specified by `LIBCXX_ROOT`, if any.
function(boost_hana_add_executable name)
    add_executable(${ARGV})
    if (DEFINED LIBCXX_ROOT)
        target_link_libraries(${name} ${libcxx})
        set_property(TARGET ${name} APPEND PROPERTY
            INCLUDE_DIRECTORIES "${LIBCXX_ROOT}/include/c++/v1")
    endif()
endfunction()

# Note: benchmark/ MUST be included before doc/ because the 'doc' target
# depends on the 'benchmarks' target.
enable_testing()
include_directories(include)
add_subdirectory(benchmark)
add_subdirectory(doc)
add_subdirectory(example)
add_subdirectory(test)
