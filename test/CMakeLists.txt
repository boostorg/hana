# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#=============================================================================
# Setup unit tests
#=============================================================================

# Allows putting stuff that's only relevant to the unit tests in the
# test/include subdirectory.
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_custom_target(tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
    COMMENT "Build and run all the unit tests.")

file(GLOB_RECURSE BOOST_HANA_TESTS_REQUIRING_BOOST
    "ext/boost/*.cpp"
    "record/macros.cpp"
    "bugs/fold_move_only_record.cpp")

list(APPEND BOOST_HANA_FILES_REQUIRING_BOOST ${BOOST_HANA_TESTS_REQUIRING_BOOST})

file(GLOB_RECURSE BOOST_HANA_TEST_SOURCES "*.cpp")
foreach(file IN LISTS BOOST_HANA_TEST_SOURCES)
    boost_hana_add_test(${file} target)
    if (TARGET ${target})
        add_dependencies(tests ${target})
    endif()
endforeach()


# Add tests for individual header inclusions.
add_custom_target(test.headers ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.header.+"
    COMMENT "Compile all the header-inclusion unit tests.")
add_dependencies(tests test.headers)

function(boost_hana_add_header_test header)
    string(REGEX REPLACE "/" "." header_name "${header}")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp" "
        #include <${header}>
        int main() { }
    ")
    boost_hana_add_executable(test.header.${header_name}
                        "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
    if (TARGET test.header.${header_name})
        add_test(NAME test.header.${header_name} COMMAND test.header.${header_name})
        add_dependencies(test.headers test.header.${header_name})
    endif()
endfunction()

file(GLOB_RECURSE _public_headers
    RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${CMAKE_CURRENT_LIST_DIR}/../include/*.hpp")

file(GLOB_RECURSE _detail_headers
    RELATIVE "${CMAKE_CURRENT_LIST_DIR}/../include"
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/detail/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/sandbox/*.hpp")
list(REMOVE_ITEM _public_headers ${_detail_headers})

if (NOT Boost_FOUND)
    file(GLOB_RECURSE _headers_depending_on_boost
        RELATIVE "${PARENT_DIRECTORY}/include"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/ext/boost.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/fwd/ext/boost/*.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/../include/boost/hana/fwd/ext/boost.hpp")
    list(REMOVE_ITEM _public_headers ${_headers_depending_on_boost})
endif()

foreach(header IN LISTS _public_headers)
    boost_hana_add_header_test("${header}")
endforeach()
