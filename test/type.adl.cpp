/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/type.hpp>
using namespace boost::hana;


template <bool b = false>
struct invalid { static_assert(b, "invalid must not be instantiated"); };

template <typename T> void adl(T) { }
template <typename T> void adl_pattern(basic_type<T>) { }


int main() {
    // ADL kicks in but `invalid<>` must not instantiated
    adl(type_c<invalid<>>);
    adl_pattern(type_c<invalid<>>);

    // ADL instantiates the types recursively, make sure that works too
    adl(decltype_(type_c<invalid<>>));
    adl_pattern(decltype_(type_c<invalid<>>));
}
