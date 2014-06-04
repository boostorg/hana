/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct T; struct E;

int main() {
    BOOST_HANA_STATIC_ASSERT(if_(true_, int_<0>, int_<1>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(if_(false_, int_<0>, int_<1>) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(if_(true, 0, 1) == 0);
    BOOST_HANA_STATIC_ASSERT(if_(false, 0, 1) == 1);

    BOOST_HANA_STATIC_ASSERT(if_(true_, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(if_(false_, type<T>, type<E>) == type<E>);
}
