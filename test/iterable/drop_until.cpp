/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/iterable/mcd.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/iterable.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto iterable = detail::minimal::iterable<mcd>;

    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable()) == iterable());

    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable(true_)) == iterable(true_));
    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable(false_)) == iterable());

    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable(true_, true_)) == iterable(true_, true_));
    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable(true_, false_)) == iterable(true_, false_));
    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable(false_, true_)) == iterable(true_));
    BOOST_HANA_CONSTANT_ASSERT(drop_until(id, iterable(false_, false_)) == iterable());
}

int main() {
    test<Iterable::mcd>();
}
