/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/type_list.hpp>
using namespace boost::hana;


int main() {
    //! [comparing]
    BOOST_HANA_CONSTEXPR_LAMBDA auto grouped = group_by(comparing(length), list(
        list(1, 2, 3),
        list('x', 'y', 'z'),
        range_c<long, 0, 1>,
        type_list<char, int>,
        range_c<int, 0, 2>,
        list(123.4, nullptr)
    ));

    BOOST_HANA_CONSTEXPR_ASSERT(grouped == list(
        list(
            list(1, 2, 3),
            list('x', 'y', 'z')
        ),
        list(
            range_c<long, 0, 1>
        ),
        list(
            type_list<char, int>,
            range_c<int, 0, 2>,
            list(123.4, nullptr)
        )
    ));
    //! [comparing]

    //! [equal]
    BOOST_HANA_CONSTEXPR_ASSERT(equal(list(1, 2), list(1, 2)));
    BOOST_HANA_CONSTEXPR_ASSERT(!equal('x', 'y'));
    BOOST_HANA_CONSTANT_ASSERT(!equal(list(1, 2), 'y'));
    //! [equal]

    //! [not_equal]
    BOOST_HANA_CONSTEXPR_ASSERT(not_equal(list(1, 2), list(3)));
    BOOST_HANA_CONSTEXPR_ASSERT(not_equal('x', 'y'));
    BOOST_HANA_CONSTANT_ASSERT(not_equal(list(1, 2), 'y'));
    //! [not_equal]
}
