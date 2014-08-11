/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>

#include "comparable.hpp"
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(f, lazy(1)), lazy(f(1))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(f, lazy(2.2)), lazy(f(2.2))));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(id, lazy(1)), lazy(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(id, lazy('2')), lazy('2')));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(id, lazy(3.3)), lazy(3.3)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
            list(lazy(x<0>)), list(f), list(g)
        ));
    }
}
