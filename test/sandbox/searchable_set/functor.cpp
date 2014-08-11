/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, singleton(x<0>)) == singleton(f(x<0>)));
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, doubleton(x<0>, x<1>)) == doubleton(f(x<0>), f(x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, doubleton(x<0>, x<0>)) == singleton(f(x<0>)));
    }
}
