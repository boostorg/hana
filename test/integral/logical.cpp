/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/integral.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct T; struct E;
constexpr struct { } invalid{};

auto test_eval_if = [](auto true_, auto false_) {
    auto x = eval_if(true_,
        [](auto) { return type<T>; },
        [](auto id) { id(invalid)("would fail"); return type<E>; }
    );
    BOOST_HANA_CONSTANT_ASSERT(x == type<T>);

    auto y = eval_if(false_,
        [](auto id) { id(invalid)("would fail"); return type<T>; },
        [](auto) { return type<E>; }
    );
    BOOST_HANA_CONSTANT_ASSERT(y == type<E>);
};

auto test_if = [](auto true_, auto false_) {
    BOOST_HANA_CONSTANT_ASSERT(if_(true_, type<T>, type<E>) == type<T>);
    BOOST_HANA_CONSTANT_ASSERT(if_(false_, type<T>, type<E>) == type<E>);
};

auto test_and = [](auto true_, auto false_) {
    BOOST_HANA_CONSTANT_ASSERT(!or_(false_));
    BOOST_HANA_CONSTANT_ASSERT(or_(true_));
    BOOST_HANA_CONSTANT_ASSERT(!or_(false_, false_));
    BOOST_HANA_CONSTANT_ASSERT(or_(false_, true_));
    BOOST_HANA_CONSTANT_ASSERT(or_(false_, false_, true_));
    // BOOST_HANA_CONSTANT_ASSERT(or_(true_, invalid, invalid));
};

auto test_or = [](auto true_, auto false_) {
    BOOST_HANA_CONSTANT_ASSERT(and_(true_));
    BOOST_HANA_CONSTANT_ASSERT(!and_(false_));
    BOOST_HANA_CONSTANT_ASSERT(and_(true_, true_));
    BOOST_HANA_CONSTANT_ASSERT(!and_(true_, false_));
    BOOST_HANA_CONSTANT_ASSERT(!and_(true_, true_, false_));
    // BOOST_HANA_CONSTANT_ASSERT(!and_(false_, invalid, invalid));
};

auto test_not = [](auto true_, auto false_) {
    BOOST_HANA_CONSTANT_ASSERT(not_(false_));
    BOOST_HANA_CONSTANT_ASSERT(not_(not_(true_)));
};

auto test = [](auto true_, auto false_) {
    test_eval_if(true_, false_);
    test_if(true_, false_);
    test_and(true_, false_);
    test_or(true_, false_);
    test_not(true_, false_);
};

int main() {
    test(true_, false_);
    test(int_<2>, int_<0>); // make sure int_<2> can be treated as true_
}
