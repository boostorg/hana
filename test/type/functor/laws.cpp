/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functor/laws.hpp>
using namespace boost::hana;


template <typename ...> struct f;
template <typename ...> struct g;
struct x;

int main() {
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(type<x>, template_<f>, template_<g>));
    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(type<void>, template_<f>, template_<g>));
}
