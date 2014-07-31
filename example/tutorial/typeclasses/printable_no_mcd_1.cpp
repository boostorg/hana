/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <iostream>
#include <type_traits>
using namespace boost::hana;


//! [Printable]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);
};
//! [Printable]

namespace counter_examples_and_stuff {
//! [Printable_dumb]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);

    // This is valid, but it's probably not a good idea unless those are
    // strongly related to the type class.
    int foo;
    void bar() const { };
    struct baz { };
};
//! [Printable_dumb]
}

//! [print]
auto print = [](std::ostream& os, auto x) {
    return Printable::instance<decltype(x)>::print_impl(os, x);
};
//! [print]

//! [int_instance]
template <>
struct Printable::instance<int> {
    static void print_impl(std::ostream& os, int i) {
        os << i;
    }
};
//! [int_instance]

//! [arithmetic_instance]
template <typename T>
struct Printable::instance<T, when<std::is_arithmetic<T>::value>> {
    static void print_impl(std::ostream& os, T x) {
        os << x;
    }
};
//! [arithmetic_instance]


int main() {
    //! [print_int]
print(std::cout, 2);
    //! [print_int]

    //! [print_double]
print(std::cout, 2.2);
    //! [print_double]
}
