/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x + int_<1>; };
BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return x * int_<3>; };

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(f, argwise(list))() == list());
    BOOST_HANA_STATIC_ASSERT(fmap(f, argwise(list))(int_<0>) == list(f(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, argwise(list))(int_<0>, int_<1>) == list(f(int_<0>), f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, argwise(list))(int_<0>, int_<1>, int_<2>) == list(f(int_<0>), f(int_<1>), f(int_<2>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, argwise(list))(0, 1, 2, 3) == list(f(0), f(1), f(2), f(3)));

    // Note: Functor laws can't be checked automatically because that would
    // require comparing function objects, which is not implemented.
    BOOST_HANA_CONSTEXPR_LAMBDA auto test_laws = [](auto ...xs) {
        BOOST_HANA_STATIC_ASSERT(fmap(id, argwise(list))(xs...) == list(xs...));
        BOOST_HANA_STATIC_ASSERT(
            fmap(f, fmap(g, argwise(list)))(xs...) ==
            fmap(compose(f, g), argwise(list))(xs...)
        );
    };
    test_laws();
    test_laws(int_<0>);
    test_laws(0, int_<1>, 2);
}
