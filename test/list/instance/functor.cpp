/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functor/detail/laws.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});
    constexpr auto x = detail::number<>;

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            fmap(f, list()),
            list()
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(f, list(x(1))),
            list(f(x(1)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(f, list(x(1), x(2))),
            list(f(x(1)), f(x(2)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(f, list(x(1), x(2), x(3))),
            list(f(x(1)), f(x(2)), f(x(3)))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
            list(
                list(),
                list(x(0)),
                list(x(0), x(1)),
                list(x(0), x(1), x(2))
            ),
            list(f),
            list(g)
        ));
    }
}
