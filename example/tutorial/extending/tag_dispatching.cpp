/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/datatype.hpp>

#include <iostream>


struct UserDefinedDatatype { };

//! [main]
template <typename Datatype>
struct print_impl {
    template <typename X>
    static void apply(std::ostream& os, X x) {
        /* possibly some default implementation */
    }
};

template <typename X>
void print(std::ostream& os, X x) {
    using Datatype = typename boost::hana::datatype<X>::type;
    print_impl<Datatype>::apply(os, x);
}

template <>
struct print_impl<UserDefinedDatatype> {
    template <typename X>
    static void apply(std::ostream& os, X x) {
        /* a custom implementation for some data type */
    }
};
//! [main]

int main() {
    print(std::cout, UserDefinedDatatype{}); // just make sure it works
}
