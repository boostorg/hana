/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/is_subset.hpp>
#include <boost/hana/set.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto even = hana::make_set(
        <%= (1..input_size).map { |n| "hana::int_c<#{n*2}>" }.join(', ') %>
    );
    constexpr auto all = hana::make_set(
        <%= (1..input_size*2).map { |n| "hana::int_c<#{n}>" }.join(', ') %>
    );
    hana::is_subset(even, all);
}
