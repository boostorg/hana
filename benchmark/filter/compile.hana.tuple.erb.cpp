/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/equal.hpp>
#include <boost/hana/filter.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/rem.hpp>
#include <boost/hana/tuple.hpp>


struct is_even {
    template <typename N>
    constexpr auto operator()(N n) const {
        return n % boost::hana::int_c<2> == boost::hana::int_c<0>;
    }
};

int main() {
    constexpr auto tuple = boost::hana::make_tuple(
        <%= (1..input_size).map { |n| "boost::hana::int_c<#{n}>" }.join(', ') %>
    );
    constexpr auto result = boost::hana::filter(tuple, is_even{});
    (void)result;
}
