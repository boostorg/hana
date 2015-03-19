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
option(BOOST_HANA_ENABLE_CXX1Y    "Use the -std=c++1y switch if the compiler supports it." ON)
option(BOOST_HANA_ENABLE_LIBCXX   "Use the -stdlib=libc++ if the compiler supports it." ON)


##############################################################################
# Setup compiler flags
##############################################################################
include(CheckCXXCompilerFlag)
macro(boost_hana_append_flag lst testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        list(APPEND ${lst} ${flag})
    endif()
endmacro()

boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_W_FLAG                         -W)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WALL_FLAG                      -Wall)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WEXTRA_FLAG                    -Wextra)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WNO_LONG_LONG_FLAG             -Wno-long-long)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG -Wno-unused-local-typedefs)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WNO_UNUSED_PARAMETER_FLAG      -Wno-unused-parameter)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WWRITE_STRINGS_FLAG            -Wwrite-strings)
boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_FTEMPLATE_BACKTRACE_LIMIT_FLAG -ftemplate-backtrace-limit=0)

check_cxx_compiler_flag("-Xclang -fdefine-sized-deallocation" BOOST_HANA_HAS_FDEFINE_SIZED_DEALLOCATION_FLAG)
if (${BOOST_HANA_HAS_FDEFINE_SIZED_DEALLOCATION_FLAG})
    list(APPEND BOOST_HANA_CXX_FEATURE_FLAGS -Xclang -fdefine-sized-deallocation)
endif()

if (BOOST_HANA_ENABLE_WERROR)
    boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WERROR_FLAG -Werror)
    boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_WX_FLAG     -WX)
endif()

if (BOOST_HANA_ENABLE_PEDANTIC)
    boost_hana_append_flag(BOOST_HANA_CXX_WARNING_FLAGS BOOST_HANA_HAS_PEDANTIC_FLAG -pedantic)
endif()

if (BOOST_HANA_ENABLE_CXX1Y)
    boost_hana_append_flag(BOOST_HANA_CXX_FEATURE_FLAGS BOOST_HANA_HAS_STDCXX1Y_FLAG -std=c++1y)
endif()

if (BOOST_HANA_ENABLE_LIBCXX)
    boost_hana_append_flag(BOOST_HANA_CXX_FEATURE_FLAGS BOOST_HANA_HAS_STDLIB_LIBCXX_FLAG -stdlib=libc++)
endif()

# This is the only place where `add_compile_options` is called.
# Other properties are set on a per-target basis.
add_compile_options(
    ${BOOST_HANA_CXX_WARNING_FLAGS}
    ${BOOST_HANA_CXX_FEATURE_FLAGS}
)


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
endif()


##############################################################################
# Caveats
##############################################################################
if (NOT Boost_FOUND)
    message(STATUS
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

# Note: benchmark/ MUST be included before doc/ because the 'doc' target
# depends on the 'benchmarks' target.
enable_testing()
include_directories(include)
add_subdirectory(benchmark)
add_subdirectory(doc)
add_subdirectory(example)
add_subdirectory(test)
