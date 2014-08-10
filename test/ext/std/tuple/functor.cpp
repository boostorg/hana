/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functor/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    // fmap
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
        constexpr auto x = detail::number<>;
        BOOST_HANA_CONSTANT_ASSERT(equal(fmap(f, std::make_tuple()), std::make_tuple()));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(f, std::make_tuple(x(1))), std::make_tuple(f(x(1)))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(f, std::make_tuple(x(1), x(2))), std::make_tuple(f(x(1)), f(x(2)))));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(fmap(f, std::make_tuple(x(1), x(2), x(3))), std::make_tuple(f(x(1)), f(x(2)), f(x(3)))));
    }

    // laws
    {
        constexpr auto x = detail::number<>;
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});
        BOOST_HANA_CONSTEXPR_LAMBDA auto h = detail::injection([]{});
        BOOST_HANA_CONSTEXPR_LAMBDA auto i = detail::injection([]{});
        BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
            list(
                std::make_tuple(),
                std::make_tuple(x(0)),
                std::make_tuple(x(0), x(1)),
                std::make_tuple(x(0), x(1), x(2))
            ),
            list(f, g),
            list(h, i)
        ));
    }
}
