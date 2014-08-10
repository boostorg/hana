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
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/monad/detail/laws.hpp>
using namespace boost::hana;


constexpr auto x = detail::number<>;

int main() {
    // flatten
    {
        BOOST_HANA_CONSTANT_ASSERT(flatten(nothing) == nothing);
        BOOST_HANA_CONSTANT_ASSERT(flatten(just(nothing)) == nothing);
        BOOST_HANA_CONSTEXPR_ASSERT(flatten(just(just(x(0)))) == just(x(0)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = compose(just, detail::injection([]{}));
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = compose(just, detail::injection([]{}));
        BOOST_HANA_CONSTEXPR_ASSERT(Monad::laws::check(
            list(
                nothing,
                just(x(0)),
                just(x(1))
            ),
            list(
                x(0), x(1)
            ),
            list(f),
            list(g)
        ));
    }
}
