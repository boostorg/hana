/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/wrong.hpp>
using namespace boost::hana;


//! [wrong]
template <typename T, typename U>
struct base_template {
    // Can't write this because the assertion would always fire up:
    // static_assert(false, "...");

    // So instead we write this:
    static_assert(wrong<base_template<T, U>>{},
    "base_template does not have a valid default definition");
};

template <>
struct base_template<int, int> {
    // something useful
};
//! [wrong]

int main() { }
