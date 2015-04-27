# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup examples
##############################################################################

add_custom_target(compile.examples COMMENT "Build all the examples.")
add_dependencies(compile.all compile.examples)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_custom_target(run.examples
    COMMENT "Build and then run all the examples."
    DEPENDS compile.examples
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "example.+")
add_dependencies(run.all run.examples)

if (Valgrind_FOUND)
    add_custom_target(memcheck.examples
        COMMENT "Build and then run all the examples under the memory checker.")
    add_dependencies(memcheck.all memcheck.examples)
endif()

# A list of all the example files
file(GLOB_RECURSE BOOST_HANA_EXAMPLE_SOURCES "*.cpp")

# If Boost is not found, remove the examples that require Boost from the list.
if (NOT Boost_FOUND)
    file(GLOB_RECURSE _examples_that_require_Boost
        "ext/boost/*.cpp"
        "misc/mini_mpl.cpp"
        "struct.cpp"
        "struct.macros.cpp"
        "tutorial/mpl_cheatsheet.cpp"
        "tutorial/type.cpp"
    )
    list(REMOVE_ITEM BOOST_HANA_EXAMPLE_SOURCES ${_examples_that_require_Boost})
endif()

# Add all the examples
foreach(file IN LISTS BOOST_HANA_EXAMPLE_SOURCES)
    boost_hana_target_name_for(_target "${file}")
    boost_hana_add_executable(${_target} EXCLUDE_FROM_ALL "${file}")
    add_dependencies(compile.examples compile.${_target})
    add_test(NAME ${_target} COMMAND compile.${_target})
    if (Valgrind_FOUND)
        add_dependencies(memcheck.examples memcheck.${_target})
    endif()
endforeach()
