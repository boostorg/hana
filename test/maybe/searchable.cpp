/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/constant.hpp>
#include <boost/hana/detail/constexpr.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::cnumber<int, i>;

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
        return [=](auto y) { return equal(x, y); };
    };

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(find(just(x<0>), is(x<0>)) == just(x<0>));
        BOOST_HANA_CONSTANT_ASSERT(find(just(x<0>), is(x<1>)) == nothing);
        BOOST_HANA_CONSTANT_ASSERT(find(nothing, is(x<0>)) == nothing);
    }

    // any
    {
        BOOST_HANA_CONSTANT_ASSERT( any(just(x<0>), is(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(!any(just(x<0>), is(x<1>)));
        BOOST_HANA_CONSTANT_ASSERT( !any(nothing, is(x<0>)));
    }
}
