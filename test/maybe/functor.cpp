/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functor/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


int main() {
    constexpr auto x = detail::number<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT( equal(fmap(f, nothing), nothing));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(f, just(x(0))), just(f(x(0)))));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
            list(nothing, just(x(0))),
            list(f),
            list(g)
        ));
    }
}
