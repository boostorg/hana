# Copyright Louis Dionne 2015
# with contributions of Markus J. Weber 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module checks the full implementation of the C++ standard library
# by performing a compilation check. If anything goes wrong, most likely an
# incomplete implementation of C++14 is used from GCC < 5.1.0.
#
# These checks include code in the file <type_traits>
#    - the 'std::is_trivially_*' traits
#    - the 'aligned_union' struct
#
# If an incomplete libstdc++ is detected, a CMake variable and macro symbol is set
#    BOOST_HANA_CXX_INCOMPLETE_STDLIB = 1

include(CheckCXXSourceRuns)
check_cxx_source_runs("
// Check whether the used C++ standard library implements trivially type traits
#include <type_traits>
#include <iostream>

#define CHECK_TRAIT(METHOD)  \
    if (METHOD::value) \
        std::cout << #METHOD << std::endl;

int main () {
    // Check for a basic type trait. If this doesn't work, something weird is wrong.
    CHECK_TRAIT(std::is_integral<int>)

    CHECK_TRAIT(std::is_trivially_copyable<int>)
    CHECK_TRAIT(std::is_trivially_constructible<int>)
    CHECK_TRAIT(std::is_trivially_default_constructible<int>)
    CHECK_TRAIT(std::is_trivially_copy_constructible<int>)
    CHECK_TRAIT(std::is_trivially_move_constructible<int>)

    if (std::is_trivially_assignable<int,int>::value) \
        std::cout << \"std::is_trivially_assignable<int,int>\" << std::endl;
    CHECK_TRAIT(std::is_trivially_copy_assignable<int>)
    CHECK_TRAIT(std::is_trivially_move_assignable<int>)

    CHECK_TRAIT(std::is_trivially_destructible<int>)

    union U { int i; char c; double d; };
    using au_t = std::aligned_union<sizeof(U),int,char,double>::type;

    return 0;
}
" CXX_COMPLETE_STDLIBC14)

if (NOT CXX_COMPLETE_STDLIBC14)
    message("
    ### Your C++ standard library has an incomplete implementation of
    ### the C++14 type traits in <type_traits>. The depending code is
    ### automatically deactivated in Boost::Hana and all tests by
    ### adding the definition -DBOOST_HANA_CXX_INCOMPLETE_STDLIB.
    ")
    set(BOOST_HANA_CXX_INCOMPLETE_STDLIB 1 CACHE BOOL
        "An incomplete libstdc++ has been detected." FORCE)
    add_definitions( -DBOOST_HANA_CXX_INCOMPLETE_STDLIB )
endif()

# Remove the temporary cmake variable to show this warning every time.
unset(CXX_COMPLETE_STDLIBC14)
