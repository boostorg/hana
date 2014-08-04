/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


//! [Printable]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);
};
//! [Printable]

auto to_string = [](auto x) {
    return Printable::instance<
        datatype_t<decltype(x)>
    >::to_string_impl(x);
};

auto print = [](std::ostream& os, auto x) {
    return Printable::instance<
        datatype_t<decltype(x)>
    >::print_impl(os, x);
};

//! [int_instance]
template <>
struct Printable::instance<int> {
    static void print_impl(std::ostream& os, int i) {
        os << i;
    }

    static std::string to_string_impl(int i) {
        return (std::ostringstream{} << i).str();
    }
};
//! [int_instance]

//! [arithmetic_instance]
template <typename T>
struct Printable::instance<T, when<std::is_arithmetic<T>::value>> {
    static void print_impl(std::ostream& os, T x) {
        os << x;
    }

    static std::string to_string_impl(T x) {
        return (std::ostringstream{} << x).str();
    }
};
//! [arithmetic_instance]


int main() {
    //! [print_int]
print(std::cout, 2);
assert(to_string(2) == "2");
    //! [print_int]

    //! [print_double]
print(std::cout, 2.2);
assert(to_string(2.2) == "2.2");
    //! [print_double]
}
