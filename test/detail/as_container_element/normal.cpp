/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/as_container_element.hpp>

#include <type_traits>
namespace hana = boost::hana;
using hana::detail::as_container_element_t;


struct T { };

static_assert(std::is_same<T, as_container_element_t<T>>{}, "");
static_assert(std::is_same<T, as_container_element_t<T const>>{}, "");
static_assert(std::is_same<T, as_container_element_t<T&>>{}, "");
static_assert(std::is_same<T, as_container_element_t<T const&>>{}, "");
static_assert(std::is_same<T, as_container_element_t<T&&>>{}, "");
static_assert(std::is_same<T, as_container_element_t<T const&&>>{}, "");

int main() { }
