/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


//! @todo
//! We use `decltype_(T{})` instead of `type<T>` because Clang segfaults.
//! See http://llvm.org/bugs/show_bug.cgi?id=20046
//! and use `type<T>` once that's fixed.
struct T { }; struct E { };
constexpr struct { } invalid{};

void test_eval_if() {
    BOOST_HANA_STATIC_ASSERT(eval_if(true_,
        [](auto) { return decltype_(T{}); },
        [](auto id) { id(invalid)("would fail"); return decltype_(E{}); }
    ) == type<T>);

    BOOST_HANA_STATIC_ASSERT(eval_if(false_,
        [](auto id) { id(invalid)("would fail"); return decltype_(T{}); },
        [](auto) { return decltype_(E{}); }
    ) == type<E>);
}

void test_if() {
    BOOST_HANA_STATIC_ASSERT(if_(true_, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(if_(false_, type<T>, type<E>) == type<E>);
}

void test_and() {
    BOOST_HANA_STATIC_ASSERT(!or_(false_));
    BOOST_HANA_STATIC_ASSERT(or_(true_));
    BOOST_HANA_STATIC_ASSERT(!or_(false_, false_));
    BOOST_HANA_STATIC_ASSERT(or_(false_, true_));
    BOOST_HANA_STATIC_ASSERT(or_(false_, false_, true_));
    // BOOST_HANA_STATIC_ASSERT(or_(true_, invalid, invalid));
}

void test_or() {
    BOOST_HANA_STATIC_ASSERT(and_(true_));
    BOOST_HANA_STATIC_ASSERT(!and_(false_));
    BOOST_HANA_STATIC_ASSERT(and_(true_, true_));
    BOOST_HANA_STATIC_ASSERT(!and_(true_, false_));
    BOOST_HANA_STATIC_ASSERT(!and_(true_, true_, false_));
    // BOOST_HANA_STATIC_ASSERT(!and_(false_, invalid, invalid));
}

int main() {
    test_eval_if();
    test_if();
    test_and();
    test_or();
}
