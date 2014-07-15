/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>
using namespace boost::hana;


template <typename X>
auto function(X x) {
    return to<int>(x);
}

int main() {
    // Clang asserts in the code generation if `to` is
    // implemented as a lambda.
    function(3);
}
