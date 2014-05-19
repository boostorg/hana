/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    static_assert(std::is_same<
        decltype(lift<std::add_pointer>(int{}))::type,
        int*
    >::value, "");
}
