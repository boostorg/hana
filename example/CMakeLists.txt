# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#=============================================================================
# Setup examples
#=============================================================================

# Target to build and run all the examples
add_custom_target(examples ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "example.+"
    COMMENT "Build and run all the examples.")

# A list of all the example files
file(GLOB_RECURSE BOOST_HANA_EXAMPLE_SOURCES "*.cpp")

# If Boost is not found, remove the examples that require Boost from the list.
if (NOT Boost_FOUND)
    file(GLOB_RECURSE _examples_that_require_Boost
        "ext/boost/*.cpp"
        "record.macros.cpp"
        "tutorial/type.cpp"
        "tutorial/mpl_cheatsheet.cpp"
        "mini_mpl.cpp")
    list(REMOVE_ITEM BOOST_HANA_EXAMPLE_SOURCES ${_examples_that_require_Boost})
endif()

# Add all the examples
foreach(file IN LISTS BOOST_HANA_EXAMPLE_SOURCES)
    boost_hana_target_name_for(_target "${file}")
    add_executable(${_target} EXCLUDE_FROM_ALL "${file}")
    target_link_libraries(${_target} c++)
    add_test(NAME ${_target} COMMAND ${_target})
    add_dependencies(examples ${_target})
endforeach()
