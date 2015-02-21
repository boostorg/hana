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
    "record.macros.cpp"
    "tutorial/type_computations.cpp")
list(APPEND BOOST_HANA_FILES_REQUIRING_BOOST ${BOOST_HANA_EXAMPLES_REQUIRING_BOOST})

file(GLOB_RECURSE BOOST_HANA_EXAMPLE_SOURCES "*.cpp")

foreach(file IN LISTS BOOST_HANA_EXAMPLE_SOURCES)
    boost_hana_add_test(${file} target)
    if (TARGET ${target})
        add_dependencies(examples ${target})
    endif()
endforeach()
