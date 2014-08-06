/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/comparable/not_equal_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto comp = detail::minimal::comparable<mcd>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto comp_ = [=](auto i) {
        return [=] { return comp(i); };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto equal_ = comparing([](auto x) {
        return x();
    });

    BOOST_HANA_CONSTEXPR_ASSERT(equal_(comp_(0), comp_(1)) == equal(comp(0), comp(1)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal_(comp_(0), comp_(0)) == equal(comp(0), comp(0)));
    BOOST_HANA_CONSTEXPR_ASSERT(equal_(comp_(1), comp_(0)) == equal(comp(1), comp(0)));
}

int main() {
    test<Comparable::equal_mcd>();
    test<Comparable::not_equal_mcd>();
}
