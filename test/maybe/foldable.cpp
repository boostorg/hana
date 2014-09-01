/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe/comparable.hpp>
#include <boost/hana/maybe/foldable.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto x = test::injection([]{})();
    BOOST_HANA_CONSTEXPR_LAMBDA auto y = test::injection([]{})();
    BOOST_HANA_CONSTEXPR_LAMBDA auto s = test::injection([]{})();
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // foldl
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl(just(x), s, f), f(s, x)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl(nothing, s, f), s));
    }

    // foldr
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr(just(x), s, f), f(x, s)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr(nothing, s, f), s));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            nothing, just(x), just(y)
        ));
    }
}
