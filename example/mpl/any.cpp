/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>

#include <type_traits>
using namespace boost::hana;


struct odd {
    template <typename T>
    constexpr auto operator()(T t) const { return t % int_<2>; }
};

static_assert(std::is_same<
    decltype(any(odd{}, list(int_<1>, int_<2>))),
    Bool<true>
>::value, "");

static_assert(std::is_same<
    decltype(any(odd{}, list(int_<2>, int_<4>))),
    Bool<false>
>::value, "");


int main() { }
