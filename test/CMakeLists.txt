# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#=============================================================================
# Setup unit tests
#=============================================================================
# Allows putting stuff that's only relevant to the unit tests in the test/
# subdirectory and including those headers as:
#   #include <test/something.hpp>
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_custom_target(tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
    COMMENT "Build and run all the unit tests.")

file(GLOB_RECURSE BOOST_HANA_TESTS_REQUIRING_BOOST
    "ext/boost/*.cpp"
    "record/macros.cpp")

list(APPEND BOOST_HANA_FILES_REQUIRING_BOOST ${BOOST_HANA_TESTS_REQUIRING_BOOST})

file(GLOB_RECURSE BOOST_HANA_TEST_SOURCES "*.cpp")

foreach(file IN LISTS BOOST_HANA_TEST_SOURCES)
    file(RELATIVE_PATH rel_file ${CMAKE_CURRENT_LIST_DIR} ${file})
    string(REGEX REPLACE "\\.cpp$" "" name ${rel_file})
    string(REGEX REPLACE "/" "." name ${name})
    boost_hana_add_executable(test.${name} "${file}")
    if (TARGET test.${name})
        add_test(NAME test.${name} COMMAND test.${name})
        add_dependencies(tests test.${name})
    endif()
endforeach()
