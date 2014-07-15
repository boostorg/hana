/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/iterable.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;

    BOOST_HANA_STATIC_ASSERT(find(always(true_), iterable()) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(always(true_), iterable(x<0>)) == just(x<0>));
    BOOST_HANA_STATIC_ASSERT(find(always(false_), iterable(x<0>)) == nothing);

    BOOST_HANA_STATIC_ASSERT(find(id, iterable(true_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(true_, false_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(false_, true_)) == just(true_));
    BOOST_HANA_STATIC_ASSERT(find(id, iterable(false_, false_)) == nothing);
}

int main() {
    test<Iterable::mcd>();
}
