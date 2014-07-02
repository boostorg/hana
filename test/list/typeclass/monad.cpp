/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>

#include "minimal.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(), minimal_list())) == minimal_list());
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(int_<0>), minimal_list())) == minimal_list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(), minimal_list(int_<0>))) == minimal_list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(minimal_list(minimal_list(int_<0>), minimal_list(int_<1>))) == minimal_list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(
        flatten(minimal_list(minimal_list(0, int_<1>), minimal_list(), minimal_list('2', 3.3), minimal_list(int_<4>)))
        ==
        minimal_list(0, int_<1>, '2', 3.3, int_<4>)
    );

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return minimal_list(x + int_<1>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return minimal_list(x * int_<3>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto check_monad_laws = [=](auto ...xs) {
        BOOST_HANA_STATIC_ASSERT(detail::laws<Monad>(minimal_list(xs...), int_<1>, f, g));
    };
    check_monad_laws();
    check_monad_laws(int_<1>);
    check_monad_laws(int_<1>, int_<2>);
    check_monad_laws(1, 2, 3, 4);
}
