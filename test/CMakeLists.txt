# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup master test targets and file lists
##############################################################################

# This allows putting stuff that's only relevant to the unit tests in the
# test/include subdirectory.
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_custom_target(compile.tests
    COMMENT "Build all the unit tests.")
add_dependencies(compile.all compile.tests)

add_custom_target(run.tests
    COMMENT "Build and then run all the unit tests."
    DEPENDS compile.tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+")
add_dependencies(run.all run.tests)

add_custom_target(compile.headers
    COMMENT "Build all the header-inclusion unit tests.")
add_dependencies(compile.tests compile.headers)

if (Valgrind_FOUND)
    add_custom_target(memcheck.tests
        COMMENT "Build and then run all the tests under the memory checker.")
    add_dependencies(memcheck.all memcheck.tests)
endif()

# A list of all the test files
# Source files that are not available due to caveats are removed from this
# list below.
file(GLOB_RECURSE BOOST_HANA_TEST_SOURCES "*.cpp")

# A list of all the public headers.
# Header files that are not available due to caveats are removed from this
# list below.
file(GLOB_RECURSE BOOST_HANA_PUBLIC_HEADERS
    RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${CMAKE_CURRENT_LIST_DIR}/../include/*.hpp")
file(GLOB_RECURSE _detail_headers
    RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/detail/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost/fusion/detail/*.hpp")
list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS ${_detail_headers})


##############################################################################
# Caveats
##############################################################################

# If Boost is not found, remove the headers that require Boost from the list
# of public headers and the tests that require Boost from the list of tests.
if (NOT Boost_FOUND)
    file(GLOB_RECURSE _tests_that_require_Boost
        "ext/boost/*.cpp"
        "struct_macros.*.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_tests_that_require_Boost})

    file(GLOB_RECURSE _headers_that_require_boost
        RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/fwd/ext/boost/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/fwd/ext/boost.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/struct_macros.hpp")
    list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS ${_headers_that_require_boost})
endif()

if (Boost_FOUND)
    if (${Boost_VERSION} LESS 105800)
        # If the version of Boost is < 1.58.0, the fusion::deque and
        # fusion::list adapters are not being tested because of bugs
        # in Boost.Fusion.
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

    # Currently, the tests for fusion::deque's models of MonadPlus and Sequence
    # are disabled due to bugs in Fusion.
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/deque/monad_plus.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/deque/sequence.cpp")

    # Currently, the tests for fusion::list's models of Monad, MonadPlus and
    # Sequence are disabled due to bugs in Fusion.
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/list/monad.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/list/monad_plus.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/list/sequence.cpp")

    # Currently, the tests for fusion::tuple's models of Monad and Sequence
    # are disabled because of bugs in Fusion.
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/tuple/monad.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/tuple/sequence.cpp")

    # Currently, the test for fusion::vector's model of Sequence is disabled
    # because of a bug in Fusion.
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/vector/sequence.cpp")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND
    "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "3.7.0")
    # The std::tuple adapter is not supported with Clang < 3.7.0
    list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS
        "boost/hana/fwd/ext/std/tuple.hpp"
        "boost/hana/ext/std/tuple.hpp")

    file(GLOB_RECURSE _ext_std_tuple_tests "ext/std/tuple.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_ext_std_tuple_tests})
endif()


##############################################################################
# Generate tests that include each public header.
##############################################################################
foreach(header IN LISTS BOOST_HANA_PUBLIC_HEADERS)
    string(REGEX REPLACE "/" "." header_name "${header}")
    if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp" "
            #include <${header}>
            int main() { }
        ")
    endif()
    boost_hana_add_executable(header.${header_name} EXCLUDE_FROM_ALL
                        "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
    add_dependencies(compile.headers compile.header.${header_name})

    add_test(NAME header.${header_name} COMMAND compile.header.${header_name})
endforeach()


##############################################################################
# Add other custom unit tests
##############################################################################
# Check for ODR violations when linking several translation units
# (GitHub issue 75)
list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/bugs/github_75.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/bugs/github_75_other_tu.cpp")

boost_hana_target_name_for(github_75 "${CMAKE_CURRENT_LIST_DIR}/bugs/github_75.cpp")
boost_hana_add_executable(${github_75} EXCLUDE_FROM_ALL
                        "bugs/github_75.cpp" "bugs/github_75_other_tu.cpp")
add_test(NAME ${github_75} COMMAND compile.${github_75})


##############################################################################
# Add all the unit tests
##############################################################################
foreach(_file IN LISTS BOOST_HANA_TEST_SOURCES)
    file(READ "${_file}" _contents)

    # If the file contains the special BOOST_HANA_TEST_PART marker, we split
    # it into several subparts. This is done to avoid long compile-times and
    # other inconveniences.
    if ("${_contents}" MATCHES "BOOST_HANA_TEST_PART == ([0-9]+)")
        string(REGEX MATCHALL "BOOST_HANA_TEST_PART == ([0-9]+)" _parts "${_contents}")
        string(REGEX REPLACE "BOOST_HANA_TEST_PART == ([0-9]+)" "\\1" _parts "${_parts}")
        list(REMOVE_DUPLICATES _parts)

        boost_hana_target_name_for(_target "${_file}")
        add_custom_target(compile.${_target})
        add_dependencies(compile.tests compile.${_target})
        add_custom_target(run.${_target})
        if (Valgrind_FOUND)
            add_custom_target(memcheck.${_target})
            add_dependencies(memcheck.tests memcheck.${_target})
        endif()

        foreach(_part IN LISTS _parts)
            set(_ptarget "${_target}.part${_part}")
            boost_hana_add_executable(${_ptarget} EXCLUDE_FROM_ALL "${_file}")
            set_target_properties(compile.${_ptarget}
                PROPERTIES COMPILE_DEFINITIONS "BOOST_HANA_TEST_PART=${_part}")
            add_dependencies(compile.${_target} compile.${_ptarget})
            add_dependencies(run.${_target} run.${_ptarget})

            if (Valgrind_FOUND)
                add_dependencies(memcheck.${_target} memcheck.${_ptarget})
            endif()

            add_test(NAME ${_ptarget} COMMAND compile.${_ptarget})
        endforeach()
    else()
        boost_hana_target_name_for(_target "${_file}")
        boost_hana_add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
        add_dependencies(compile.tests compile.${_target})
        add_test(NAME ${_target} COMMAND compile.${_target})
        if (Valgrind_FOUND)
            add_dependencies(memcheck.tests memcheck.${_target})
        endif()
    endif()
endforeach()
