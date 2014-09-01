/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set/comparable.hpp>
#include <boost/hana/set/convert.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp> //! @todo get rid of this
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>
#include <test/laws/comparable.hpp>
#include <test/numeric/comparable.hpp>
#include <test/numeric/logical.hpp>
using namespace boost::hana;


int main() {
    using test::x;
    auto n = test::numeric;

    // equal
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto check = [](auto ...keys) {
            auto eq = [=](auto s) { return equal(to<Set>(s), set(keys...)); };
            return and_(
                all(permutations(tuple(keys...)), eq),
                not_(equal(set(keys...), set(keys..., x<999>)))
            );
        };

        BOOST_HANA_CONSTANT_ASSERT(check());
        BOOST_HANA_CONSTANT_ASSERT(check(x<0>));
        BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>));
        BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>, x<2>));

        BOOST_HANA_CONSTEXPR_ASSERT(check(n(0)));
        BOOST_HANA_CONSTEXPR_ASSERT(check(n(0), n(1)));
        BOOST_HANA_CONSTEXPR_ASSERT(check(n(0), n(1), n(2)));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            set(),
            set(x<0>),
            set(x<0>, x<1>),
            set(x<1>, x<0>),
            set(x<0>, x<1>, x<2>)
        ));
    }
}
