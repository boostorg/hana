/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type_list.hpp>
using namespace boost::hana;


int main() {
    //! [comparing]
    BOOST_HANA_CONSTEXPR_LAMBDA auto grouped = group_by(comparing(length), tuple(
        tuple(1, 2, 3),
        tuple('x', 'y', 'z'),
        range_c<long, 0, 1>,
        type_list<char, int>,
        range_c<int, 0, 2>,
        tuple(123.4, nullptr)
    ));

    BOOST_HANA_CONSTEXPR_ASSERT(grouped == tuple(
        tuple(
            tuple(1, 2, 3),
            tuple('x', 'y', 'z')
        ),
        tuple(
            range_c<long, 0, 1>
        ),
        tuple(
            type_list<char, int>,
            range_c<int, 0, 2>,
            tuple(123.4, nullptr)
        )
    ));
    //! [comparing]

    //! [equal]
    BOOST_HANA_CONSTEXPR_ASSERT(equal(tuple(1, 2), tuple(1, 2)));
    BOOST_HANA_CONSTEXPR_ASSERT(!equal('x', 'y'));
    BOOST_HANA_CONSTANT_ASSERT(!equal(tuple(1, 2), 'y'));
    //! [equal]

    //! [not_equal]
    BOOST_HANA_CONSTEXPR_ASSERT(not_equal(tuple(1, 2), tuple(3)));
    BOOST_HANA_CONSTEXPR_ASSERT(not_equal('x', 'y'));
    BOOST_HANA_CONSTANT_ASSERT(not_equal(tuple(1, 2), 'y'));
    //! [not_equal]
}
