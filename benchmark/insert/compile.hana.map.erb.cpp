/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/insert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto map = hana::make_map(
        <%= (1..input_size).map { |n|
            "hana::make_pair(hana::int_c<#{n}>, hana::int_c<#{n}>)"
        }.join(', ') %>
    );
    constexpr auto next = hana::int_c< <%= input_size+1 %> >;
    constexpr auto result = hana::insert(map, hana::make_pair(next, next));
    (void)result;
}
