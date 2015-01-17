/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
using namespace boost::hana;


template <typename X>
void test(X x) {
    to<int>(x);
    is_a<int>(x);
    is_an<int>(x);
    is<int>(x);
    models<int>(x);
}

int main() {
    // Clang asserts in the code generation when we use variable template
    // lambdas inside a template.
    test(3);
}
