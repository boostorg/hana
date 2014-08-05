/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/convert.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <typename X, typename Y, typename Z>
struct Triple {
    X first;
    Y second;
    Z third;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto triple = [](auto x, auto y, auto z) {
    return Triple<decltype(x), decltype(y), decltype(z)>{x, y, z};
};

namespace boost { namespace hana {
    template <typename X, typename Y, typename Z>
    struct convert<List, Triple<X, Y, Z>> {
        static constexpr auto apply(Triple<X, Y, Z> xs) {
            return list(xs.first, xs.second, xs.third);
        }
    };
}}

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(to<List>(triple(1, '2', 3.3)) == list(1, '2', 3.3));
}
