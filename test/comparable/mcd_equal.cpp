/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include "minimal.hpp"
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<MinimalComparable, MinimalComparable>
        : Comparable::equal_mcd
    {
        template <int i, int j>
        static constexpr auto equal_impl(_comparable<i>, _comparable<j>)
        { return bool_<i == j>; }
    };
}}

int main() {
    BOOST_HANA_STATIC_ASSERT(not_equal(comparable<0>, comparable<1>));
    BOOST_HANA_STATIC_ASSERT(!(not_equal(comparable<0>, comparable<0>)));

    using operators::operator!=;
    BOOST_HANA_STATIC_ASSERT(comparable<0> != comparable<1>);
    BOOST_HANA_STATIC_ASSERT(!(comparable<0> != comparable<0>));
}
