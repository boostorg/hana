/*
@copyright Jason Rice 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at_key.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


int main() {
    constexpr auto tuple = hana::make_tuple(
        <%= (0...input_size).map { |n| "hana::int_c<#{n}>" }.join(', ') %>
    );
    constexpr auto result = hana::at_key(tuple, hana::int_c<<%= input_size-1 %>>);
    (void)result;
}
