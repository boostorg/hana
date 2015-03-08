# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#=============================================================================
# Setup unit tests
#=============================================================================

# Allows putting stuff that's only relevant to the unit tests in the
# test/include subdirectory.
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Target to build and run all the unit tests
add_custom_target(tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
    COMMENT "Build and run all the unit tests.")

# Target to build an run all the invididual header inclusion tests
add_custom_target(test.headers ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.header.+"
    COMMENT "Compile all the header-inclusion unit tests.")
add_dependencies(tests test.headers)

# A list of all the test files
file(GLOB_RECURSE BOOST_HANA_TEST_SOURCES "*.cpp")

# A list of all the public headers
file(GLOB_RECURSE BOOST_HANA_PUBLIC_HEADERS
    RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${CMAKE_CURRENT_LIST_DIR}/../include/*.hpp")
file(GLOB_RECURSE _detail_headers
    RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/detail/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/sandbox/*.hpp")
list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS ${_detail_headers})

# If Boost is not found, remove the headers that require Boost from the list
# of public headers and the tests that require Boost from the list of tests.
if (NOT Boost_FOUND)
    file(GLOB_RECURSE _tests_that_require_Boost
        "ext/boost/*.cpp"
        "record.macros.cpp"
        "record.fold_move_only.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_tests_that_require_Boost})

    file(GLOB_RECURSE _headers_that_require_boost
        RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/fwd/ext/boost/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/fwd/ext/boost.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/record_macros.hpp")
    list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS ${_headers_that_require_boost})
endif()

# With Boost < 1.58.0, the fusion::{deque,list} adapters are not supported.
if (Boost_FOUND)
    if (${Boost_VERSION} LESS 105800)
        list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS
            "boost/hana/ext/boost/fusion/deque.hpp"
            "boost/hana/fwd/ext/boost/fusion/deque.hpp"
            "boost/hana/ext/boost/fusion/list.hpp"
            "boost/hana/fwd/ext/boost/fusion/list.hpp")

        file(GLOB_RECURSE _ext_fusion_bad_tests
            "ext/boost/fusion/deque/*.cpp"
            "ext/boost/fusion/list/*.cpp")
        list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_ext_fusion_bad_tests})
    endif()
endif()

# With Clang < 3.7.0, the std::tuple adapter is not supported.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND
    "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "3.7.0")
    list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS
        "boost/hana/fwd/ext/std/tuple.hpp"
        "boost/hana/ext/std/tuple.hpp")

    file(GLOB_RECURSE _ext_std_tuple_tests "ext/std/tuple/*.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_ext_std_tuple_tests})
endif()

# The Fusion adapters are currently broken; I'm working on it.
file(GLOB_RECURSE _ext_fusion_broken_tests "ext/boost/fusion/*.cpp")
list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_ext_fusion_broken_tests})

# Add all the unit tests
foreach(file IN LISTS BOOST_HANA_TEST_SOURCES)
    boost_hana_target_name_for(_target "${file}")
    add_executable(${_target} EXCLUDE_FROM_ALL "${file}")
    add_test(NAME ${_target} COMMAND ${_target})
    add_dependencies(tests ${_target})
endforeach()

# Generate tests that include each public header and add them to the
# test.headers target.
foreach(header IN LISTS BOOST_HANA_PUBLIC_HEADERS)
    string(REGEX REPLACE "/" "." header_name "${header}")
    if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp" "
            #include <${header}>
            int main() { }
        ")
    endif()
    add_executable(test.header.${header_name} EXCLUDE_FROM_ALL
                        "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
    add_test(NAME test.header.${header_name} COMMAND test.header.${header_name})
    add_dependencies(test.headers test.header.${header_name})
endforeach()
