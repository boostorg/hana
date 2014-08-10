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
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/monad/detail/laws.hpp>

#include <tuple>
using namespace boost::hana;


int main() {
    constexpr auto x = detail::number<>;

    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            flatten(std::make_tuple(std::make_tuple(), std::make_tuple())),
            std::make_tuple()
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            flatten(std::make_tuple(std::make_tuple(x(0)), std::make_tuple())),
            std::make_tuple(x(0))
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            flatten(std::make_tuple(std::make_tuple(), std::make_tuple(x(0)))),
            std::make_tuple(x(0))
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            flatten(std::make_tuple(std::make_tuple(x(0)), std::make_tuple(x(1)))),
            std::make_tuple(x(0), x(1))
        ));
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            flatten(std::make_tuple(std::make_tuple(0, x(1)), std::make_tuple(), std::make_tuple('2', 3.3), std::make_tuple(x(4)))),
            std::make_tuple(0, x(1), '2', 3.3, x(4))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto make = [](auto ...xs)
        { return std::make_tuple(xs...); };
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(make, detail::injection([]{}));
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = compose(make, detail::injection([]{}));
        BOOST_HANA_CONSTEXPR_ASSERT(Monad::laws::check(
            list(
                std::make_tuple(),
                std::make_tuple(x(1)),
                std::make_tuple(x(1), x(2)),
                std::make_tuple(1, 2, 3, 4)
            ),
            list(
                x(1), x(2), x(3)
            ),
            list(f),
            list(g)
        ));
    }
}
