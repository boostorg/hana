#=============================================================================
# Setup project
#=============================================================================
project(Boost.Hana CXX)
cmake_minimum_required(VERSION 2.8)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(BOOST_HANA_IS_NESTED FALSE)
else()
    set(BOOST_HANA_IS_NESTED TRUE)
endif()


#=============================================================================
# Setup CMake options
#=============================================================================
include(CMakeDependentOption)
option(BOOST_HANA_ENABLE_PEDANTIC "Compile with pedantic enabled." ON)
option(BOOST_HANA_ENABLE_WERROR   "Fail and stop if a warning is triggered." OFF)
option(BOOST_HANA_ENABLE_CXX1Y    "Use the -std=c++1y switch if the compiler supports it." ON)
option(BOOST_HANA_ENABLE_LIBCXX   "Use the -stdlib=libc++ if the compiler supports it." ON)


#=============================================================================
# Check available compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-pedantic                    BOOST_HANA_HAS_PEDANTIC_FLAG)
check_cxx_compiler_flag(-std=c++1y                   BOOST_HANA_HAS_STDCXX1Y_FLAG)
check_cxx_compiler_flag(-stdlib=libc++               BOOST_HANA_HAS_STDLIB_LIBCXX_FLAG)
check_cxx_compiler_flag(-W                           BOOST_HANA_HAS_W_FLAG)
check_cxx_compiler_flag(-Wall                        BOOST_HANA_HAS_WALL_FLAG)
check_cxx_compiler_flag(-Werror                      BOOST_HANA_HAS_WERROR_FLAG)
check_cxx_compiler_flag(-Wextra                      BOOST_HANA_HAS_WEXTRA_FLAG)
check_cxx_compiler_flag(-Wno-long-long               BOOST_HANA_HAS_WNO_LONG_LONG_FLAG)
check_cxx_compiler_flag(-Wno-unused-local-typedefs   BOOST_HANA_HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG)
check_cxx_compiler_flag(-Wno-unused-parameter        BOOST_HANA_HAS_WNO_UNUSED_PARAMETER_FLAG)
check_cxx_compiler_flag(-Wwrite-strings              BOOST_HANA_HAS_WWRITE_STRINGS_FLAG)


#=============================================================================
# Setup warning flags
#=============================================================================
macro(append_if condition lst var)
    if (${condition})
        list(APPEND ${lst} ${var})
    endif()
endmacro()
append_if(BOOST_HANA_HAS_WALL_FLAG                      BOOST_HANA_CXX_WARNING_FLAGS -Wall)
append_if(BOOST_HANA_HAS_WEXTRA_FLAG                    BOOST_HANA_CXX_WARNING_FLAGS -Wextra)
append_if(BOOST_HANA_HAS_W_FLAG                         BOOST_HANA_CXX_WARNING_FLAGS -W)
append_if(BOOST_HANA_HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG BOOST_HANA_CXX_WARNING_FLAGS -Wno-unused-local-typedefs)
append_if(BOOST_HANA_HAS_WNO_UNUSED_PARAMETER_FLAG      BOOST_HANA_CXX_WARNING_FLAGS -Wno-unused-parameter)
append_if(BOOST_HANA_HAS_WWRITE_STRINGS_FLAG            BOOST_HANA_CXX_WARNING_FLAGS -Wwrite-strings)
append_if(BOOST_HANA_HAS_WNO_LONG_LONG_FLAG             BOOST_HANA_CXX_WARNING_FLAGS -Wno-long-long)

if (BOOST_HANA_ENABLE_WERROR)
    append_if(BOOST_HANA_HAS_WERROR_FLAG BOOST_HANA_CXX_WARNING_FLAGS -Werror)
    append_if(BOOST_HANA_HAS_WX_FLAG     BOOST_HANA_CXX_WARNING_FLAGS -WX)
endif()

if (BOOST_HANA_ENABLE_PEDANTIC)
    append_if(BOOST_HANA_HAS_PEDANTIC_FLAG BOOST_HANA_CXX_WARNING_FLAGS -pedantic)
endif()


#=============================================================================
# Setup feature flags
#=============================================================================
if (BOOST_HANA_ENABLE_CXX1Y AND BOOST_HANA_HAS_STDCXX1Y_FLAG)
    list(APPEND BOOST_HANA_CXX_FEATURE_FLAGS -std=c++1y)
endif()

if (BOOST_HANA_ENABLE_LIBCXX AND BOOST_HANA_HAS_STDLIB_LIBCXX_FLAG)
    list(APPEND BOOST_HANA_CXX_FEATURE_FLAGS -stdlib=libc++)
endif()

# This is the only place where add_definitions is called. Other properties
# are set on a per-target basis.
add_definitions(
    ${BOOST_HANA_CXX_WARNING_FLAGS}
    ${BOOST_HANA_CXX_FEATURE_FLAGS}
)


#=============================================================================
# Search for packages.
#
# Behavior when the package is found or not is customized at the
# point where the package is required.
#=============================================================================
find_package(Doxygen)
find_package(Gnuplot)
find_package(Ruby 2.1)


#=============================================================================
# Setup subdirectories
#=============================================================================
include_directories(include)
add_subdirectory(benchmarks)
add_subdirectory(doc)
add_subdirectory(example)
add_subdirectory(test)
