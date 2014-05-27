/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


template <typename ...> struct f;
template <typename T> using g = Type<f<T>>;
struct x { };

int main() {
    BOOST_HANA_STATIC_ASSERT(unit<_Type>(x{}) == type<x>);

    BOOST_HANA_STATIC_ASSERT(join(type<Type<x>>) == type<x>);
    BOOST_HANA_STATIC_ASSERT(join(type<Type<Type<x>>>) == type<Type<x>>);

    BOOST_HANA_STATIC_ASSERT(bind(type<x>, template_<g>) == type<f<x>>);
}
