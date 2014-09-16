# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#=============================================================================
# Setup examples
#=============================================================================
add_custom_target(examples ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "example.+"
    COMMENT "Build and run all the examples.")

file(GLOB_RECURSE BOOST_HANA_EXAMPLES_REQUIRING_BOOST
    "ext/boost/*.cpp"
    "record/macros.cpp")
list(APPEND BOOST_HANA_FILES_REQUIRING_BOOST ${BOOST_HANA_EXAMPLES_REQUIRING_BOOST})

file(GLOB_RECURSE BOOST_HANA_EXAMPLE_SOURCES "*.cpp")

foreach(file IN LISTS BOOST_HANA_EXAMPLE_SOURCES)
    file(RELATIVE_PATH rel_file ${CMAKE_CURRENT_LIST_DIR} ${file})
    string(REGEX REPLACE "\\.cpp$" "" name ${rel_file})
    string(REGEX REPLACE "/" "." name ${name})
    boost_hana_add_executable(example.${name} "${file}")
    if (TARGET example.${name})
        add_test(NAME example.${name} COMMAND example.${name})
        add_dependencies(examples example.${name})
    endif()
endforeach()
