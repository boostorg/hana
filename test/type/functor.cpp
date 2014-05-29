/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename ...> struct f;
template <typename ...> struct g;
struct x;

int main() {
    BOOST_HANA_STATIC_ASSERT(fmap(template_<f>, type<x>) == type<f<x>>);
    BOOST_HANA_STATIC_ASSERT(fmap(template_<f>, type<void>) == type<f<void>>);

    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(type<x>, template_<f>, template_<g>));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(type<void>, template_<f>, template_<g>));
}
