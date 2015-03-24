# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup master test targets and file lists
##############################################################################

# Allows putting stuff that's only relevant to the unit tests in the
# test/include subdirectory.
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

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
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/sandbox/*.hpp")
list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS ${_detail_headers})

# A list of all the unit tests that are split into subparts to reduce the
# load on the compiler.
#
# This list must not be modified by the caveats below; when a source file is
# not available in BOOST_HANA_TEST_SOURCES, it won't be split regardless of
# whether it appears in this list.
file(GLOB BOOST_HANA_SPLIT_TEST_SOURCES
    "cnumeric.cpp"
    "detail/canonical_constant.cpp"
    "either.cpp"
    "ext/boost/mpl/integral_c.cpp"
    "ext/std/array.cpp"
    "ext/std/integer_sequence.cpp"
    "ext/std/integral_constant.cpp"
    "ext/std/ratio.cpp"
    "ext/std/tuple.cpp"
    "integral_constant.cpp"
    "maybe.cpp"
    "minimal_record.cpp"
    "set.cpp"
    "string.cpp"
    "tuple.cpp"
)


##############################################################################
# Caveats
##############################################################################

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

if (Boost_FOUND)
    if (${Boost_VERSION} LESS 105800)
        message(STATUS "The Boost version was detected to be < 1.58.0; "
                       "the fusion::deque and fusion::list adapters are "
                       "not being tested because of bugs in Boost.Fusion.")

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

    message(STATUS "The fusion::tuple model of Monad is not being tested "
                   "currently because of a bug in Boost.Fusion.")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/ext/boost/fusion/tuple/monad.cpp")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND
    "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "3.7.0")
    message(STATUS "The compiler was detected to be Clang < 3.7.0; "
                   "the std::tuple adapter will not be supported.")

    list(REMOVE_ITEM BOOST_HANA_PUBLIC_HEADERS
        "boost/hana/fwd/ext/std/tuple.hpp"
        "boost/hana/ext/std/tuple.hpp")

    file(GLOB_RECURSE _ext_std_tuple_tests "ext/std/tuple.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_ext_std_tuple_tests})
endif()

if ($ENV{TRAVIS})
    # The sandbox/lambda_tuple.cpp test causes internal compiler errors on
    # Travis, so it's disabled there for now.
    file(GLOB_RECURSE _sandbox_lambda_tuple_test "sandbox/lambda_tuple.cpp")
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_sandbox_lambda_tuple_test})

    # For the MCD unit tests, we only keep the tests that directly
    # relate to the tested MCD to reduce the total build time and
    # the risk of OOM failure on Travis.
    file(GLOB_RECURSE _non_minimal_mcds
        "sequence/*iterable.cpp"
        "sequence/*monad*.cpp"
        "sequence/*orderable.cpp"
        "sequence/*searchable.cpp"

        "foldable/*monad*.cpp"
        "foldable/*orderable.cpp"
        "foldable/*searchable.cpp"
        "foldable/*sequence.cpp"
    )
    list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${_non_minimal_mcds})
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
    add_executable(compile.header.${header_name} EXCLUDE_FROM_ALL
                        "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
    add_dependencies(compile.headers compile.header.${header_name})

    add_test(NAME header.${header_name} COMMAND compile.header.${header_name})
endforeach()


##############################################################################
# Split specially marked unit tests into subparts to avoid long compile-times,
# OOMs on Travis and other inconveniences.
##############################################################################
foreach(_file IN LISTS BOOST_HANA_SPLIT_TEST_SOURCES)
    # We only process the file if it appears in the list of all tests,
    # which means it was not removed in the caveats above.
    list(FIND BOOST_HANA_TEST_SOURCES "${_file}" _index)
    if (NOT (${_index} EQUAL -1))
        # Parse the file to find how it is split.
        file(READ "${_file}" _contents)
        string(REGEX MATCHALL "BOOST_HANA_TEST_PART == ([0-9]+)" _parts "${_contents}")
        string(REGEX REPLACE "BOOST_HANA_TEST_PART == ([0-9]+)" "\\1" _parts "${_parts}")
        list(REMOVE_DUPLICATES _parts)

        boost_hana_target_name_for(_target "${_file}")
        add_custom_target(compile.${_target})
        add_dependencies(compile.tests compile.${_target})
        add_custom_target(run.${_target})

        foreach(_part IN LISTS _parts)
            set(_ptarget "${_target}.part${_part}")
            add_executable(compile.${_ptarget} EXCLUDE_FROM_ALL "${_file}")
            set_target_properties(compile.${_ptarget}
                PROPERTIES COMPILE_DEFINITIONS "BOOST_HANA_TEST_PART=${_part}")
            target_link_libraries(compile.${_ptarget} c++)
            add_dependencies(compile.${_target} compile.${_ptarget})

            add_custom_target(run.${_ptarget} COMMAND compile.${_ptarget})
            add_dependencies(run.${_target} run.${_ptarget})

            add_test(NAME ${_ptarget} COMMAND compile.${_ptarget})
        endforeach()
    endif()
endforeach()

# We remove the unit tests that were split into subparts so we don't
# add them twice.
list(REMOVE_ITEM BOOST_HANA_TEST_SOURCES ${BOOST_HANA_SPLIT_TEST_SOURCES})


##############################################################################
# Add all the remaining regular unit tests
##############################################################################
foreach(file IN LISTS BOOST_HANA_TEST_SOURCES)
    boost_hana_target_name_for(_target "${file}")
    add_executable(compile.${_target} EXCLUDE_FROM_ALL "${file}")
    target_link_libraries(compile.${_target} c++)
    add_dependencies(compile.tests compile.${_target})
    add_custom_target(run.${_target} COMMAND compile.${_target})

    add_test(NAME ${_target} COMMAND compile.${_target})
endforeach()
