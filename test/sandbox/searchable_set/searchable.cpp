/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/constant.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

template <int i>
constexpr auto c = detail::cnumber<int, i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
    return [=](auto y) { return equal(x, y); };
};

int main() {
    // any
    {
        BOOST_HANA_CONSTEXPR_ASSERT( any(singleton(x<0>), is(x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(!any(singleton(x<0>), is(x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT( any(doubleton(x<0>, x<1>), is(x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT( any(doubleton(x<0>, x<1>), is(x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(!any(doubleton(x<0>, x<1>), is(x<2>)));
    }

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(find(singleton(c<0>), is(c<0>)) == just(c<0>));
        BOOST_HANA_CONSTANT_ASSERT(find(singleton(c<1>), is(c<0>)) == nothing);

        BOOST_HANA_CONSTANT_ASSERT(find(doubleton(c<0>, c<1>), is(c<0>)) == just(c<0>));
        BOOST_HANA_CONSTANT_ASSERT(find(doubleton(c<0>, c<1>), is(c<1>)) == just(c<1>));
        BOOST_HANA_CONSTANT_ASSERT(find(doubleton(c<0>, c<1>), is(c<2>)) == nothing);
    }
}
