/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{
//! [comparable]
BOOST_HANA_CONSTANT_CHECK(nothing == nothing);
BOOST_HANA_CONSTEXPR_CHECK(just('x') == just('x'));
BOOST_HANA_CONSTEXPR_CHECK(just('x') != just('y'));
BOOST_HANA_CONSTANT_CHECK(just('x') != nothing);
//! [comparable]

}{

//! [orderable]
BOOST_HANA_CONSTANT_CHECK(nothing < just(3));
BOOST_HANA_CONSTANT_CHECK(just(0) > nothing);
BOOST_HANA_CONSTEXPR_CHECK(just(1) < just(3));
BOOST_HANA_CONSTEXPR_CHECK(just(3) > just(2));
//! [orderable]

}{

//! [functor]
BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + 1; };
BOOST_HANA_CONSTANT_CHECK(transform(nothing, inc) == nothing);
BOOST_HANA_CONSTEXPR_CHECK(transform(just(1), inc) == just(2));
//! [functor]

}{

//! [applicative]
BOOST_HANA_CONSTEXPR_CHECK(ap(just(succ), just('x')) == just('y'));
BOOST_HANA_CONSTANT_CHECK(ap(nothing, just('x')) == nothing);
BOOST_HANA_CONSTANT_CHECK(ap(just(succ), nothing) == nothing);
BOOST_HANA_CONSTANT_CHECK(ap(nothing, nothing) == nothing);
//! [applicative]

}{

//! [monad]
BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) {
    return just(x + 1);
};

BOOST_HANA_CONSTEXPR_CHECK(bind(just(1), inc) == just(2));
BOOST_HANA_CONSTANT_CHECK(bind(nothing, inc) == nothing);

BOOST_HANA_CONSTEXPR_CHECK(flatten(just(just(2))) == just(2));
//! [monad]

}{

//! [monad_plus]
BOOST_HANA_CONSTEXPR_CHECK(
    concat(nothing, just('x')) == just('x')
);

BOOST_HANA_CONSTANT_CHECK(
    concat(nothing, nothing) == nothing
);

BOOST_HANA_CONSTEXPR_CHECK(
    concat(just('x'), just('y')) == just('x')
);

BOOST_HANA_CONSTANT_CHECK(nil<Maybe>() == nothing);
//! [monad_plus]

}{

//! [traversable]
BOOST_HANA_CONSTEXPR_LAMBDA auto replicate3 = [](auto x) {
    return make<Tuple>(x, x, x);
};

BOOST_HANA_CONSTEXPR_CHECK(
    traverse<Tuple>(just(1), replicate3)
    ==
    make<Tuple>(just(1), just(1), just(1))
);

BOOST_HANA_CONSTANT_CHECK(
    traverse<Tuple>(nothing, replicate3)
    ==
    make<Tuple>(nothing)
);
//! [traversable]

}{

//! [foldable]
BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) {
    return x + y;
};

BOOST_HANA_CONSTEXPR_CHECK(foldr(nothing, 1, plus) == 1);
BOOST_HANA_CONSTEXPR_CHECK(foldr(just(4), 1, plus) == 5);
//! [foldable]

}{

//! [searchable]
BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
    return x % int_<2> != int_<0>;
};

BOOST_HANA_CONSTANT_CHECK(find(just(int_<3>), odd) == just(int_<3>));
BOOST_HANA_CONSTANT_CHECK(find(just(int_<2>), odd) == nothing);
BOOST_HANA_CONSTANT_CHECK(find(nothing, odd) == nothing);

BOOST_HANA_CONSTANT_CHECK(all(just(int_<3>), odd));
BOOST_HANA_CONSTANT_CHECK(all(nothing, odd));
//! [searchable]

}{

//! [maybe]
BOOST_HANA_CONSTEXPR_CHECK(maybe('x', succ, just(1)) == 2);
BOOST_HANA_CONSTEXPR_CHECK(maybe('x', succ, nothing) == 'x');
//! [maybe]

}{

//! [is_just]
BOOST_HANA_CONSTANT_CHECK( is_just(just('x')));
BOOST_HANA_CONSTANT_CHECK( is_just(just(nothing)));
BOOST_HANA_CONSTANT_CHECK(!is_just(nothing));
//! [is_just]

}{

//! [is_nothing]
BOOST_HANA_CONSTANT_CHECK( is_nothing(nothing));
BOOST_HANA_CONSTANT_CHECK(!is_nothing(just('x')));
BOOST_HANA_CONSTANT_CHECK(!is_nothing(just(nothing)));
//! [is_nothing]

}{

//! [from_maybe]
BOOST_HANA_CONSTEXPR_CHECK(from_maybe('x', just(1)) == 1);
BOOST_HANA_CONSTEXPR_CHECK(from_maybe('x', nothing) == 'x');
//! [from_maybe]

}{

//! [only_when]
BOOST_HANA_CONSTEXPR_LAMBDA auto even = [](auto x) {
    return x % int_<2> == int_<0>;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
    return x / int_<2>;
};

BOOST_HANA_CONSTANT_CHECK(only_when(even, half, int_<4>) == just(int_<2>));
BOOST_HANA_CONSTANT_CHECK(only_when(even, half, int_<3>) == nothing);
//! [only_when]

}{

//! [from_just]
BOOST_HANA_CONSTEXPR_CHECK(from_just(just('x')) == 'x');
// from_just(nothing); // compile-time static assertion
//! [from_just]

}{

//! [nothing]
constexpr auto x = nothing;
//! [nothing]

//! [just]
constexpr auto just_x = just('x');
//! [just]

(void)x;
(void)just_x;
}

}
