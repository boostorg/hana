/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/typeclass.hpp>
using namespace boost::hana;


template <typename ...>
struct nothing_to_do_with_Hana { };

template <typename = operators<Comparable>>
struct Hana_stuff { };

int main() {
    nothing_to_do_with_Hana<Hana_stuff<>>{} == nothing_to_do_with_Hana<Hana_stuff<>>{};
}
