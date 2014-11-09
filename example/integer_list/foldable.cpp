/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    //! [foldr]
    using namespace literals;

    BOOST_HANA_CONSTEXPR_LAMBDA auto numbers = integer_list<int, 5, -1, 0, -7, -2, 0, -5, 4>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto negatives = integer_list<int, -1, -7, -2, -5>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto keep_negatives = [](auto n, auto acc) {
        return if_(n < 0_c, cons(n, acc), acc);
    };

    BOOST_HANA_CONSTANT_CHECK(
        foldr(numbers, integer_list<int>, keep_negatives) == negatives
    );
    //! [foldr]
}
