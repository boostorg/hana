/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [list]
    BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
        return if_(x % int_<2> == int_<0>,
            just(x / int_<2>),
            nothing
        );
    };

    BOOST_HANA_STATIC_ASSERT(
        traverse<Maybe>(half, list(int_<2>, int_<4>, int_<6>))
        ==
        just(list(int_<1>, int_<2>, int_<3>))
    );

    BOOST_HANA_STATIC_ASSERT(
        traverse<Maybe>(half, list(int_<2>, int_<3>, int_<6>))
        ==
        nothing
    );
    //! [list]

    //! [maybe]
    BOOST_HANA_CONSTEXPR_LAMBDA auto twice = [](auto x) {
        return list(x, x);
    };

    BOOST_HANA_STATIC_ASSERT(
        traverse<List>(twice, just('x')) == list(just('x'), just('x'))
    );

    BOOST_HANA_STATIC_ASSERT(
        traverse<List>(twice, nothing) == list(nothing)
    );
    //! [maybe]
}
