/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


struct T { };
struct E { };
template <bool Cond>
struct pick_type {
    static constexpr auto value_t = if_(bool_<Cond>, type<T>, type<E>);
    typename decltype(value_t)::type value;
};


int main() {
    BOOST_HANA_STATIC_ASSERT(if_(true_, int_<0>, int_<1>) == int_<0>);
    BOOST_HANA_STATIC_ASSERT(if_(false_, int_<0>, int_<1>) == int_<1>);

    static_assert(std::is_same<decltype(pick_type<true>{}.value), T>::value, "");
    static_assert(std::is_same<decltype(pick_type<false>{}.value), E>::value, "");
}
