/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


int main() {

{
//! [comparable]
BOOST_HANA_CONSTEXPR_CHECK(left('x') == left('x'));
BOOST_HANA_CONSTANT_CHECK(right('x') != left('x'));
BOOST_HANA_CONSTEXPR_CHECK(right('x') == right('x'));
BOOST_HANA_CONSTEXPR_CHECK(right('x') != right('y'));
//! [comparable]

}{

//! [orderable]
// left is always less than right, regardless of what's in it
BOOST_HANA_CONSTANT_CHECK(left(2000) < right(2));

// when comparing two lefts or two rights, we compare the contents
BOOST_HANA_CONSTEXPR_CHECK(left(2) < left(2000));
BOOST_HANA_CONSTEXPR_CHECK(right(2) < right(2000));
//! [orderable]

}{

//! [functor]
auto safediv = infix([](auto x, auto y) {
    return eval_if(y == int_<0>,
        always(left(std::string{"division by zero"})),
        [=](auto _) { return right(x / _(y)); }
    );
});

auto inc = partial(plus, int_<1>);
BOOST_HANA_CONSTANT_CHECK(
    fmap(int_<6> ^safediv^ int_<3>, inc) == right(int_<3>)
);

BOOST_HANA_RUNTIME_CHECK(
    fmap(int_<6> ^safediv^ int_<0>, inc) == left("division by zero")
);
//! [functor]

}{

//! [monad]
auto safe_div = [](auto x, auto y) {
    return eval_if(y == int_<0>,
        always(left(std::string{"division by zero"})),
        [=](auto _) { return right(x / _(y)); }
    );
};

auto safe_dec = [](auto x) {
    return eval_if(x == int_<0>,
        always(left(std::string{"negative value"})),
        [=](auto _) { return right(_(x) - int_<1>); }
    );
};

BOOST_HANA_RUNTIME_CHECK(
    (safe_div(int_<4>, int_<0>) | safe_dec) == left("division by zero")
);

BOOST_HANA_RUNTIME_CHECK(
    (safe_div(int_<0>, int_<2>) | safe_dec) == left("negative value")
);

BOOST_HANA_CONSTANT_CHECK(
    (safe_div(int_<4>, int_<2>) | safe_dec) == right(int_<1>)
);
//! [monad]

}{

//! [foldable]
BOOST_HANA_CONSTANT_CHECK(unpack(left('x'), tuple) == tuple());
BOOST_HANA_CONSTEXPR_CHECK(unpack(right('x'), tuple) == tuple('x'));
//! [foldable]

}{

//! [traversable]
BOOST_HANA_CONSTEXPR_LAMBDA auto duplicate = [](auto x) {
    return tuple(x, x);
};

BOOST_HANA_CONSTEXPR_CHECK(traverse<Tuple>(left(1), duplicate) == tuple(left(1)));
BOOST_HANA_CONSTEXPR_CHECK(traverse<Tuple>(right(1), duplicate) == tuple(right(1), right(1)));
//! [traversable]

}{

//! [either]
BOOST_HANA_CONSTEXPR_CHECK(either(succ, pred, left(1)) == 2);
BOOST_HANA_CONSTEXPR_CHECK(either(succ, pred, right(1)) == 0);
//! [either]

}{

//! [left]
constexpr auto left_value = left('x');
//! [left]

//! [right]
constexpr auto right_value = right('x');
//! [right]

(void)left_value;
(void)right_value;
}

}
