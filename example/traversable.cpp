/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [sequence]
BOOST_HANA_CONSTEXPR_CHECK(
    sequence<Maybe>(make<Tuple>(just(1), just('2'), just(3.3))) ==
    just(make<Tuple>(1, '2', 3.3))
);

BOOST_HANA_CONSTANT_CHECK(
    sequence<Maybe>(make<Tuple>(just(1), nothing, just(3.3))) == nothing
);

// This is a generalized Cartesian product.
BOOST_HANA_CONSTEXPR_CHECK(
    sequence<Tuple>(make<Tuple>(make<Tuple>(1, 2, 3),
                                make<Tuple>(4),
                                make<Tuple>(5, 6)))
    ==
    make<Tuple>(
        make<Tuple>(1, 4, 5), make<Tuple>(1, 4, 6),
        make<Tuple>(2, 4, 5), make<Tuple>(2, 4, 6),
        make<Tuple>(3, 4, 5), make<Tuple>(3, 4, 6)
    )
);
//! [sequence]

}{

//! [traverse]
BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
    return if_(x % int_<2> == int_<0>,
        just(x / int_<2>),
        nothing
    );
};

BOOST_HANA_CONSTANT_CHECK(
    traverse<Maybe>(make<Tuple>(int_<2>, int_<4>, int_<6>), half)
    ==
    just(make<Tuple>(int_<1>, int_<2>, int_<3>))
);

BOOST_HANA_CONSTANT_CHECK(
    traverse<Maybe>(make<Tuple>(int_<2>, int_<3>, int_<6>), half)
    ==
    nothing
);

BOOST_HANA_CONSTEXPR_LAMBDA auto twice = [](auto x) {
    return make<Tuple>(x, x);
};

BOOST_HANA_CONSTEXPR_CHECK(
    traverse<Tuple>(just('x'), twice) == make<Tuple>(just('x'), just('x'))
);

BOOST_HANA_CONSTANT_CHECK(
    traverse<Tuple>(nothing, twice) == make<Tuple>(nothing)
);
//! [traverse]

}

}
