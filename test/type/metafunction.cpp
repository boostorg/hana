/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename ...> struct f { struct type; };
struct x1; struct x2; struct x3;

int main() {
    BOOST_HANA_STATIC_ASSERT(metafunction<f>() == type<f<>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction<f>(type<x1>) == type<f<x1>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction<f>(type<x1>, type<x2>) == type<f<x1, x2>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction<f>(type<x1>, type<x2>, type<x3>) == type<f<x1, x2, x3>::type>);
}
