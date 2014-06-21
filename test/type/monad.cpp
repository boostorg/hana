/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...> struct F;
//! @todo
//! If metafunctions need to look like that to be used with `bind`,
//! it's pretty useless.
//!
//! @bug
//! We need to use `std::remove_const_t` because `decltype(type<...>)` is
//! `const operators::_type<...>`, and when we nest `decltype_`s it creates
//! `operators::_type<const operators::_type<...>>` instead of the expected
//! `operators::_type<operators::_type<...>>`. This causes problems such as
//! failing to match the `join` implementation in our case. What's the way
//! to fix this?
template <typename T> using G = std::remove_const_t<decltype(type<F<T>>)>;
struct X { };

int main() {
    BOOST_HANA_STATIC_ASSERT(join(decltype_(decltype_(X{}))) == decltype_(X{}));
    BOOST_HANA_STATIC_ASSERT(join(decltype_(decltype_(decltype_(X{})))) == decltype_(decltype_(X{})));

    BOOST_HANA_STATIC_ASSERT(bind(type<X>, template_<G>) == type<F<X>>);
}
