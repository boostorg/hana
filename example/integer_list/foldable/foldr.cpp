/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
using namespace boost::hana;
using namespace literals;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto numbers = integer_list<int, 5, -1, 0, -7, -2, 0, -5, 4>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto negatives = integer_list<int, -1, -7, -2, -5>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto keep_negatives = [](auto n, auto acc) {
        return if_(n < 0_c, cons(n, acc), acc);
    };

    BOOST_HANA_STATIC_ASSERT(foldr(keep_negatives, integer_list<int>, numbers) == negatives);
    //! [main]
}
