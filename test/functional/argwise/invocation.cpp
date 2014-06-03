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


BOOST_HANA_CONSTEXPR_LAMBDA auto f = list;

int main() {
    BOOST_HANA_STATIC_ASSERT(argwise(f)() == f());
    BOOST_HANA_STATIC_ASSERT(argwise(f)(int_<0>) == f(int_<0>));
    BOOST_HANA_STATIC_ASSERT(argwise(f)(int_<0>, int_<1>) == f(int_<0>, int_<1>));
    BOOST_HANA_STATIC_ASSERT(argwise(f)(0, '1', "2") == f(0, '1', "2"));
}
