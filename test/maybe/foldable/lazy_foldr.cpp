/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include <tuple>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto ...x) {
    return std::make_tuple(x...);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto lf = [](auto ...x) {
    return f(x()...);
};

int main() {
    BOOST_HANA_STATIC_ASSERT(lazy_foldr(lf, int_<0>, nothing) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(lazy_foldr(lf, int_<0>, just(int_<1>)) == f(int_<1>, int_<0>));
}
