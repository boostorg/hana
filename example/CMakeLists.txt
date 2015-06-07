# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup custom functions, master targets and file lists for the examples
##############################################################################
add_custom_target(examples
    COMMENT "Build all the examples.")

# Several examples have unused parameters because the name of the parameters
# are useful for illustration, even if the implementation is not actually
# presented. We don't want to generate warnings for that or need to comment
# out all unused parameter names.
boost_hana_append_flag(BOOST_HANA_HAS_WNO_UNUSED_PARAMETER -Wno-unused-parameter)

#   boost_hana_add_example(<name> ...)
#
# Equivalent to `boost_hana_add_test`, except the created test is also added
# as a dependency of the `examples` target.
function(boost_hana_add_example name)
    boost_hana_add_test(${ARGV})
    add_dependencies(examples ${name})
endfunction()

# A list of all the example files
#
# Examples that are not available due to caveats are removed
# from this list below.
file(GLOB_RECURSE BOOST_HANA_EXAMPLE_SOURCES "*.cpp")


##############################################################################
# Include directories
##############################################################################
include_directories(${Boost.Hana_SOURCE_DIR}/include)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


##############################################################################
# Caveats
##############################################################################
if (NOT Boost_FOUND)
    boost_hana_list_remove_glob(BOOST_HANA_EXAMPLE_SOURCES GLOB_RECURSE
        "ext/boost/*.cpp"
        "struct.cpp"
        "struct.macros.cpp"
        "tutorial/appendix_mpl.cpp"
        "tutorial/integral.cpp"
        "tutorial/introspection.adapt.cpp"
        "tutorial/introspection.json.cpp"
        "tutorial/mpl_cheatsheet.cpp"
        "tutorial/quadrants.cpp"
        "tutorial/quickstart.cpp"
        "tutorial/quickstart.switchAny.cpp"
        "tutorial/rationale.container.cpp"
        "tutorial/type.cpp"
    )
endif()


##############################################################################
# Add all the examples
##############################################################################
foreach(_file IN LISTS BOOST_HANA_EXAMPLE_SOURCES)
    boost_hana_target_name_for(_target "${_file}")
    boost_hana_add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
    boost_hana_add_example(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
endforeach()
