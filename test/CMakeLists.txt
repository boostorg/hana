# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup custom functions, master targets and file lists for the unit tests
##############################################################################
add_custom_target(tests COMMENT "Build all the unit tests.")
add_dependencies(check tests)

add_custom_target(tests.quick COMMENT "Build a subset of all the unit tests to finish faster.")

add_custom_target(test.headers COMMENT "Build all the header-inclusion unit tests.")
add_dependencies(tests.quick test.headers)
add_dependencies(tests test.headers)

#   boost_hana_add_unit_test(<name> ...)
#
# Equivalent to `boost_hana_add_test`, except the test is also added as a
# dependency of the `tests` target.
function(boost_hana_add_unit_test name)
    boost_hana_add_test(${ARGV})
    add_dependencies(tests ${name})
    if ((NOT "${name}" MATCHES "\\.ext\\.") AND (NOT "${name}" MATCHES "_mcd"))
        add_dependencies(tests.quick ${name})
    endif()
endfunction()

#   boost_hana_add_header_test(<header-name>)
#
# Add a unit test for the public header file `header-name`, which must be a
# relative path from Hana's include directory, e.g. `boost/hana/tuple.hpp`.
#
# This function creates an executable named `header.header-name` and a test
# of the same name. The only source file of the executable contains an empty
# `main` and it includes the `header-name` file. This is used to make sure
# that including any public header works properly. Also, the
# `header.header-name` target is made a dependency of the `headers` target.
function(boost_hana_add_header_test header)
    string(REGEX REPLACE "/" "." _target "${header}")
    if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp" "
            #include <${header}>
            int main() { }
        ")
    endif()
    add_executable(test.header.${_target} EXCLUDE_FROM_ALL
                        "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
    boost_hana_add_test(test.header.${_target}
                            ${CMAKE_CURRENT_BINARY_DIR}/test.header.${_target})
    add_dependencies(test.headers test.header.${_target})
endfunction()

# A list of all the test files
#
# Source files that are not available due to caveats are removed
# from this list below.
file(GLOB_RECURSE BOOST_HANA_TEST_SOURCES "*.cpp")

# A list of all the public headers
#
# Header files that are not available due to caveats are removed
# from this list below.
file(GLOB_RECURSE BOOST_HANA_PUBLIC_HEADERS "${Boost.Hana_SOURCE_DIR}/include/*.hpp")
boost_hana_list_remove_glob(BOOST_HANA_PUBLIC_HEADERS GLOB_RECURSE
    "${Boost.Hana_SOURCE_DIR}/include/boost/hana/detail/*.hpp"
    "${Boost.Hana_SOURCE_DIR}/include/boost/hana/ext/boost/fusion/detail/*.hpp")


##############################################################################
# Include directories
##############################################################################
include_directories(${Boost.Hana_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


##############################################################################
# Caveats
##############################################################################
# If Boost is not found, remove the headers that require Boost from the list
# of public headers and the tests that require Boost from the list of tests.
if (NOT Boost_FOUND)
    boost_hana_list_remove_glob(BOOST_HANA_TEST_SOURCES GLOB_RECURSE
        "ext/boost/*.cpp")

    boost_hana_list_remove_glob(BOOST_HANA_PUBLIC_HEADERS GLOB_RECURSE
        "${Boost.Hana_SOURCE_DIR}/include/boost/hana/ext/boost/*.hpp"
        "${Boost.Hana_SOURCE_DIR}/include/boost/hana/ext/boost.hpp"
        "${Boost.Hana_SOURCE_DIR}/include/boost/hana/fwd/ext/boost/*.hpp"
        "${Boost.Hana_SOURCE_DIR}/include/boost/hana/fwd/ext/boost.hpp")
endif()

# The std::tuple adapter is not supported with Clang < 3.7.0
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND
    "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "3.7.0")
    boost_hana_list_remove_glob(BOOST_HANA_PUBLIC_HEADERS GLOB
        "${Boost.Hana_SOURCE_DIR}/include/boost/hana/fwd/ext/std/tuple.hpp"
        "${Boost.Hana_SOURCE_DIR}/include/boost/hana/ext/std/tuple.hpp")

    boost_hana_list_remove_glob(BOOST_HANA_TEST_SOURCES GLOB
        "ext/std/tuple.cpp")
endif()


##############################################################################
# Generate tests that include each public header
##############################################################################
foreach(_header IN LISTS BOOST_HANA_PUBLIC_HEADERS)
    file(RELATIVE_PATH _relative "${Boost.Hana_SOURCE_DIR}/include" "${_header}")
    boost_hana_add_header_test("${_relative}")
endforeach()


##############################################################################
# Check for ODR violations when linking several translation units
# (GitHub issue 75)
##############################################################################
boost_hana_list_remove_glob(BOOST_HANA_TEST_SOURCES GLOB
    "bugs/github_75.cpp"
    "bugs/github_75_other_tu.cpp")
boost_hana_target_name_for(github_75 "${CMAKE_CURRENT_LIST_DIR}/bugs/github_75.cpp")
add_executable(${github_75} EXCLUDE_FROM_ALL
                    "bugs/github_75.cpp" "bugs/github_75_other_tu.cpp")
boost_hana_add_unit_test(${github_75} ${CMAKE_CURRENT_BINARY_DIR}/${github_75})


##############################################################################
# Add all the unit tests
##############################################################################
foreach(_file IN LISTS BOOST_HANA_TEST_SOURCES)
    file(READ "${_file}" _contents)
    boost_hana_target_name_for(_target "${_file}")

    # If the file contains the special BOOST_HANA_TEST_PART marker, we split
    # it into several subparts. This is done to avoid long compile-times and
    # other inconveniences.
    if ("${_contents}" MATCHES "BOOST_HANA_TEST_PART == ([0-9]+)")
        string(REGEX MATCHALL "BOOST_HANA_TEST_PART == ([0-9]+)" _parts "${_contents}")
        string(REGEX REPLACE "BOOST_HANA_TEST_PART == ([0-9]+)" "\\1" _parts "${_parts}")
        list(REMOVE_DUPLICATES _parts)
        add_custom_target(${_target})

        foreach(n IN LISTS _parts)
            add_executable("${_target}.part${n}" EXCLUDE_FROM_ALL "${_file}")
            set_target_properties("${_target}.part${n}" PROPERTIES
                COMPILE_DEFINITIONS "BOOST_HANA_TEST_PART=${n}")
            add_dependencies(${_target} "${_target}.part${n}")

            boost_hana_add_unit_test("${_target}.part${n}"
                ${CMAKE_CURRENT_BINARY_DIR}/${_target}.part${n})
        endforeach()
    else()
        add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
        boost_hana_add_unit_test(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
    endif()
endforeach()
