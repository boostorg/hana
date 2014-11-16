/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/common.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //! [main]
    static_assert(std::is_same<
        common_t<int, float>, std::common_type_t<int, float>
    >{}, "");

    static_assert(std::is_same<
        common_t<int, Integral<int>>, int
    >{}, "");

    static_assert(std::is_same<
        common_t<int, Integral<long>>, long
    >{}, "");
    //! [main]
}
