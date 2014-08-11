/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/number/comparable.hpp>
#include <boost/hana/functor/detail/laws.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::number<>(i);

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = detail::injection([]{});

    // fmap
    {
        BOOST_HANA_CONSTANT_ASSERT(fmap(f, list()) == list());
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, list(x<1>)) == list(f(x<1>)));
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, list(x<1>, x<2>)) == list(f(x<1>), f(x<2>)));
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, list(x<1>, x<2>, x<3>)) == list(f(x<1>), f(x<2>), f(x<3>)));
        BOOST_HANA_CONSTEXPR_ASSERT(fmap(f, list(x<1>, x<2>, x<3>, x<4>)) == list(f(x<1>), f(x<2>), f(x<3>), f(x<4>)));
    }

    // laws
    {
        BOOST_HANA_CONSTEXPR_ASSERT(Functor::laws::check(
            boost::hana::list(
                list(),
                list(x<0>),
                list(x<0>, x<1>),
                list(x<0>, x<1>, x<2>)
            ),
            boost::hana::list(f),
            boost::hana::list(g)
        ));
    }
}

int main() {
    test<List::mcd<void>>();
}
