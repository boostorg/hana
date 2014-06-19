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
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(unit<List>(int_<0>) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(unit<List>(int_<1>) == list(int_<1>));
    BOOST_HANA_STATIC_ASSERT(unit<List>(3) == list(3));

    BOOST_HANA_STATIC_ASSERT(join(list(list(), list())) == list());
    BOOST_HANA_STATIC_ASSERT(join(list(list(int_<0>), list())) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(join(list(list(), list(int_<0>))) == list(int_<0>));
    BOOST_HANA_STATIC_ASSERT(join(list(list(int_<0>), list(int_<1>))) == list(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(
        join(list(list(0, int_<1>), list(), list('2', 3.3), list(int_<4>)))
        ==
        list(0, int_<1>, '2', 3.3, int_<4>)
    );

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return list(x + int_<1>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return list(x * int_<3>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto check_monad_laws = [=](auto ...xs) {
        BOOST_HANA_STATIC_ASSERT(detail::laws<Monad>(list(xs...), int_<1>, f, g));
    };
    check_monad_laws();
    check_monad_laws(int_<1>);
    check_monad_laws(int_<1>, int_<2>);
    check_monad_laws(1, 2, 3, 4);
}
