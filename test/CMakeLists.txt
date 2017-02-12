# Copyright Louis Dionne 2013-2017
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

add_custom_target(tests COMMENT "Build all the unit tests.")
add_dependencies(check tests)


##############################################################################
# Caveats: Take note of public headers and tests that are not supported.
##############################################################################
if (NOT Boost_FOUND)
    list(APPEND EXCLUDED_UNIT_TESTS
        "ext/boost/*.cpp"
        "experimental/printable/*.cpp")

    list(APPEND EXCLUDED_PUBLIC_HEADERS
        "boost/hana/ext/boost/.+.hpp"
        "boost/hana/ext/boost.hpp"
        "boost/hana/experimental/printable.hpp")
endif()

# The std::tuple adapter is not supported with Clang < 3.7.0
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND
    "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "3.7.0")

    list(APPEND EXCLUDED_UNIT_TESTS "ext/std/tuple.cpp")

    list(APPEND EXCLUDED_PUBLIC_HEADERS
        "boost/hana/fwd/ext/std/tuple.hpp"
        "boost/hana/ext/std/tuple.hpp")
endif()

# The experimental::type_name test is only supported on Clang >= 3.6 and
# AppleClang >= 7.0
if (NOT ((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND
          NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 3.6)
        OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" AND
            NOT ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 7)))
    list(APPEND EXCLUDED_PUBLIC_HEADERS
        "boost/hana/experimental/type_name.hpp")
    list(APPEND EXCLUDED_UNIT_TESTS "experimental/type_name.cpp")
endif()

# On Windows, Clang-cl emulates a MSVC bug that causes EBO not to be applied
# properly. We disable the tests that check for EBO.
if (MSVC AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    list(APPEND EXCLUDED_UNIT_TESTS
        "detail/ebo.cpp"
        "issues/github_202.cpp"
        "pair/empty_storage.cpp"
        "tuple/empty_member.cpp"
    )
endif()


##############################################################################
# Generate tests that include each public header.
# The headers that were excluded above due to caveats are ignored here.
##############################################################################
file(GLOB_RECURSE PUBLIC_HEADERS
    RELATIVE "${Boost.Hana_SOURCE_DIR}/include"
    "${Boost.Hana_SOURCE_DIR}/include/*.hpp"
)

add_custom_target(test.headers COMMENT "Build all the header-inclusion unit tests.")
add_dependencies(tests test.headers)
include(TestHeaders)
generate_standalone_header_tests(
    HEADERS ${PUBLIC_HEADERS}
    EXCLUDE ${EXCLUDED_PUBLIC_HEADERS}
            "boost/hana/detail/.+.hpp"
            "boost/hana/ext/boost/fusion/detail/.+.hpp"
    LINK_LIBRARIES hana
    MASTER_TARGET test.headers
    EXCLUDE_FROM_ALL
)


##############################################################################
# Check for ODR violations when linking several translation units
# (GitHub issue 75)
##############################################################################
list(APPEND EXCLUDED_UNIT_TESTS "issues/github_75/*.cpp")
boost_hana_target_name_for(github_75 "${CMAKE_CURRENT_LIST_DIR}/issues/github_75")
add_executable(${github_75} EXCLUDE_FROM_ALL "issues/github_75/tu1.cpp" "issues/github_75/tu2.cpp")
boost_hana_set_test_properties(${github_75})
add_test(${github_75} ${CMAKE_CURRENT_BINARY_DIR}/${github_75})
add_dependencies(tests ${github_75})


##############################################################################
# Add all the remaining unit tests
##############################################################################
file(GLOB_RECURSE UNIT_TESTS "*.cpp")
file(GLOB_RECURSE EXCLUDED_UNIT_TESTS ${EXCLUDED_UNIT_TESTS})
list(REMOVE_ITEM UNIT_TESTS ${EXCLUDED_UNIT_TESTS})

foreach(_file IN LISTS UNIT_TESTS)
    boost_hana_target_name_for(_target "${_file}")
    add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
    boost_hana_set_test_properties(${_target})
    target_include_directories(${_target} PRIVATE _include)
    add_test(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
    add_dependencies(tests ${_target})
endforeach()
