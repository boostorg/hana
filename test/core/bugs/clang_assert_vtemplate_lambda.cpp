/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>
using namespace boost::hana;


struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
};

template <typename X>
void test(X x) {
    to<int>(x);
    is_a<Typeclass>(x);
    is_an<Typeclass>(x);
}

int main() {
    // Clang asserts in the code generation when we use variable template
    // lambdas inside a template.
    test(3);
}
