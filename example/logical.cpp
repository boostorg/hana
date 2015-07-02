/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
#include <vector>
using namespace boost::hana;


//! [eval_if.fact]
template <typename N>
auto fact(N n) {
    return eval_if(n == int_<0>,
        [] { return int_<1>; },
        [=](auto _) { return n * fact(_(n) - int_<1>); }
    );
}
//! [eval_if.fact]

//! [eval_if.pointerize]
template <typename T>
auto pointerize_impl(T t) {
    return eval_if(traits::is_pointer(t),
        [=] { return t; },
        [=](auto _) { return _(traits::add_pointer)(t); }
    );
}

template <typename T>
using pointerize = decltype(pointerize_impl(type<T>));
//! [eval_if.pointerize]


int main() {

BOOST_HANA_CONSTANT_CHECK(equal(
    fact(int_<3>),
    int_<3 * 2 * 1>
));

static_assert(std::is_same<pointerize<int>::type, int*>{}, "");
static_assert(std::is_same<pointerize<int*>::type, int*>{}, "");


{

//! [and_]
BOOST_HANA_CONSTANT_CHECK(and_(true_, true_, true_, true_));
BOOST_HANA_CONSTEXPR_CHECK(!and_(true_, false, true_, true_));
//! [and_]

}{

//! [or_]
BOOST_HANA_CONSTANT_CHECK(or_(false_, false_, true_));
BOOST_HANA_CONSTANT_CHECK(!or_(false_, false_, false_));
//! [or_]

}{

//! [if_]
BOOST_HANA_CONSTEXPR_CHECK(if_(true, 1, 2) == 1);
BOOST_HANA_CONSTEXPR_CHECK(if_(false, 1, 2) == 2);

static_assert(
    if_(true_,
        make<Tuple>('t', 'r', 'u', 'e'),
        make<Tuple>('f', 'a', 'l', 's', 'e')
    )
    ==
    make<Tuple>('t', 'r', 'u', 'e')
, "");
//! [if_]

}{

//! [not_]
BOOST_HANA_CONSTANT_CHECK(not_(true_) == false_);
BOOST_HANA_CONSTEXPR_CHECK(not_(false) == true);
//! [not_]

}{

//! [eval_if.heterogeneous]
BOOST_HANA_CONSTEXPR_LAMBDA auto safe_make_unsigned = [](auto t) {
    return eval_if(traits::is_integral(t),
        [=](auto _) { return _(traits::make_unsigned)(t); },
        always(t)
    );
};

BOOST_HANA_CONSTANT_CHECK(safe_make_unsigned(type<void>) == type<void>);
BOOST_HANA_CONSTANT_CHECK(safe_make_unsigned(type<int>) == type<unsigned int>);
//! [eval_if.heterogeneous]
(void)safe_make_unsigned;

//! [eval_if.homogeneous]
BOOST_HANA_CONSTEXPR_LAMBDA auto safe_divide = [](auto x, auto y) {
    return eval_if(y == 0,
        [=](auto) { return 0; },
        [=](auto _) { return _(x) / y; }
    );
};

BOOST_HANA_CONSTEXPR_CHECK(safe_divide(6, 3) == 2);
BOOST_HANA_CONSTEXPR_CHECK(safe_divide(6, 0) == 0);
//! [eval_if.homogeneous]

}{

//! [while.homogeneous]
std::vector<int> ints;
int final_state = while_(_ < 10, 0, [&](int i) {
    ints.push_back(i);
    return i + 1;
});

// The state is known only at runtime
BOOST_HANA_RUNTIME_CHECK(final_state == 10);

BOOST_HANA_RUNTIME_CHECK(ints == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
//! [while.homogeneous]

}{

//! [while.heterogeneous]
using namespace literals;
std::vector<int> ints;
auto final_state = while_(_ < 10_c, 0_c, [&](auto i) {
    ints.push_back(i);
    return i + 1_c;
});

// The state is known at compile-time
BOOST_HANA_CONSTANT_CHECK(final_state == 10_c);

BOOST_HANA_RUNTIME_CHECK(ints == std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
//! [while.heterogeneous]

}

}
