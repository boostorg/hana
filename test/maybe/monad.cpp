/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(flatten(nothing) == nothing);
    BOOST_HANA_STATIC_ASSERT(flatten(just(nothing)) == nothing);
    BOOST_HANA_STATIC_ASSERT(flatten(just(just(int_<3>))) == just(int_<3>));


    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return just(x + int_<1>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return just(x * int_<3>); };
    BOOST_HANA_CONSTEXPR_LAMBDA auto check_monad_laws = [=](auto x) {
        BOOST_HANA_STATIC_ASSERT(detail::laws<Monad>(x, int_<1>, f, g));
    };
    check_monad_laws(nothing);
    check_monad_laws(just(int_<1>));
    check_monad_laws(just(1));
}
