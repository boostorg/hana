# Contains utilities to manipulate CMake lists.
#
# Copyright Louis Dionne 2014
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

# Returns whether some value is contained in a list.
#
#   list_contains(<list> <value> <output variable>)
#
# Specifically, <output variable> is TRUE if <value> can be found in <list>,
# and FALSE otherwise.
function(list_contains list value output_variable)
    list(FIND ${list} "${value}" _res)
    if (_res EQUAL -1)
        set(${output_variable} FALSE PARENT_SCOPE)
    else()
        set(${output_variable} TRUE PARENT_SCOPE)
    endif()
endfunction()
