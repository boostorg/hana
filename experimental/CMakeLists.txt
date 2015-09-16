# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

# Setup custom functions, master targets and file lists for the unit tests
add_custom_target(experimental COMMENT "Build all the experimental code.")
add_dependencies(check experimental)

# Include directories
include_directories(${Boost.Hana_SOURCE_DIR}/include)

# Add all the experimental targets
file(GLOB_RECURSE BOOST_HANA_EXPERIMENTAL_SOURCES "*.cpp")
foreach(file IN LISTS BOOST_HANA_EXPERIMENTAL_SOURCES)
    boost_hana_target_name_for(target "${file}")
    add_executable(${target} ${file})
    boost_hana_add_test(${target} ${CMAKE_CURRENT_BINARY_DIR}/${target})
    add_dependencies(experimental ${target})
endforeach()
