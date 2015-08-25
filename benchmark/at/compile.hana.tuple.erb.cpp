/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


template <int>
struct x { };

int main() {
    constexpr auto tuple = boost::hana::make_tuple(
        <%= (1..input_size).map { |n| "x<#{n}>{}" }.join(', ') %>
    );
    constexpr auto result = boost::hana::at_c<<%= input_size-1 %>>(tuple);
    (void)result;
}
