/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>
#include <boost/hana/integral_constant.hpp>


int main() {
    constexpr auto tuple = boost::hana::make_tuple(<%=
        (1..input_size).to_a.map{ |n| "boost::hana::ullong<#{n}>" }.join(', ')
    %>);
    constexpr auto result = boost::hana::product(tuple);
    (void)result;
}
