/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
using namespace boost::hana;


int main() {
    // operator()
    static_assert(size_t<0>() == 0, "");
    static_assert(size_t<1>() == 1, "");
    static_assert(int_<-3>() == -3, "");

    // decltype(operator())
    BOOST_HANA_STATIC_ASSERT(decltype_(size_t<0>()) == type<std::size_t>);
    BOOST_HANA_STATIC_ASSERT(decltype_(int_<-3>()) == type<int>);

    // conversions
    constexpr std::size_t a = size_t<0>, b = size_t<1>;
    static_assert(a == 0 && b == 1, "");

    constexpr int c = int_<0>, d = int_<-3>;
    static_assert(c == 0 && d == -3, "");
}
