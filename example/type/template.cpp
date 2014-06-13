/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


//! [main]
template <typename ...> struct f;
struct x;
struct y;

int main() {
    BOOST_HANA_STATIC_ASSERT(template_<f>() == type<f<>>);
    BOOST_HANA_STATIC_ASSERT(template_<f>(type<x>) == type<f<x>>);
    BOOST_HANA_STATIC_ASSERT(template_<f>(type<x>, type<y>) == type<f<x, y>>);
}
//! [main]
