/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/product.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>


int main() {
    constexpr auto tuple = boost::hana::tuple_c<
        <%= (['unsigned long long'] + (1..input_size).to_a).join(', ') %>
    >;
    constexpr auto result = boost::hana::product<
        boost::hana::IntegralConstant<unsigned long long>
    >(tuple);
    (void)result;
}
