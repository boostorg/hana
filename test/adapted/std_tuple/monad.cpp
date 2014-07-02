/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_tuple.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto tuple = [](auto ...xs) {
    return std::make_tuple(xs...);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(flatten(tuple(tuple(), tuple())) == tuple());
    BOOST_HANA_STATIC_ASSERT(flatten(tuple(tuple(int_<0>), tuple())) == tuple(int_<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(tuple(tuple(), tuple(int_<0>))) == tuple(int_<0>));
    BOOST_HANA_STATIC_ASSERT(flatten(tuple(tuple(int_<0>), tuple(int_<1>))) == tuple(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(
        flatten(tuple(tuple(0, int_<1>), tuple(), tuple('2', 3.3), tuple(int_<4>)))
        ==
        tuple(0, int_<1>, '2', 3.3, int_<4>)
    );

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return tuple(x + int_<1>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return tuple(x * int_<3>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto check_monad_laws = [=](auto ...xs) {
        BOOST_HANA_STATIC_ASSERT(detail::laws<Monad>(tuple(xs...), int_<1>, f, g));
    };
    check_monad_laws();
    check_monad_laws(int_<1>);
    check_monad_laws(int_<1>, int_<2>);
    check_monad_laws(1, 2, 3, 4);
}
