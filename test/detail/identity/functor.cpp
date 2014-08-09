/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functor/adjust_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/cnumber/logical.hpp>
#include <boost/hana/detail/identity/functor.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


constexpr auto comparable = detail::number<>;
template <bool b> constexpr auto c_logical = detail::cnumber<bool, b>;
BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});

template <typename Mcd>
void test() {
    constexpr auto functor = detail::identity<Mcd>;
    using F = detail::Identity<Mcd>;

    // adjust
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            adjust(always(c_logical<true>), f, functor(comparable(0))),
            functor(f(comparable(0)))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            adjust(always(c_logical<false>), f, functor(comparable(0))),
            functor(comparable(0))
        ));
    }

    // fill
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fill(comparable(1), functor(comparable(0))),
            functor(comparable(1))
        ));
    }

    // fmap
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            fmap(f, functor(comparable(0))),
            functor(f(comparable(0)))
        ));
    }

    // replace
    {
        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            replace(always(c_logical<true>), comparable(1), functor(comparable(0))),
            functor(comparable(1))
        ));

        BOOST_HANA_CONSTEXPR_ASSERT(equal(
            replace(always(c_logical<false>), comparable(1), functor(comparable(0))),
            functor(comparable(0))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
            list(functor(comparable(0)), functor(comparable(1))),
            list(f),
            list(g)
        ));
    }
}

int main() {
    test<Functor::fmap_mcd>();
    test<Functor::adjust_mcd>();
}
