/*
@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/sort.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto tuple = hana::make_tuple(
        <%= (1..input_size).map { |n| "hana::int_c<#{n}>" }.reverse.join(', ') %>
    );
    constexpr auto result = hana::sort(tuple);
    (void)result;
}
