/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <boost/hana/detail/minimal/constant.hpp>
using namespace boost::hana;


template <typename mcd, typename T>
struct test {
    static_assert(value(detail::minimal::constant<T, 0, mcd>) == 0, "");
    static_assert(value(detail::minimal::constant<T, 1, mcd>) == 1, "");
};

template struct test<Constant::mcd, int>;
template struct test<Constant::mcd, long>;

int main() { }
