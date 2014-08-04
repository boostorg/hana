/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <iostream>
#include <string>


struct UserDefinedDatatype { };

//! [main]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);

    template <typename T>
    struct default_implementation {
        template <typename X>
        static void print_impl(std::ostream& os, X x) {
            /* possibly some default implementation */;
        }

        template <typename X>
        static std::string to_string_impl(X x) {
            return "possibly some default implementation";
        }
    };
};

template <typename X>
void print(std::ostream& os, X x) {
    using Datatype = typename boost::hana::datatype<X>::type;
    Printable::instance<Datatype>::print_impl(os, x);
}

template <typename X>
std::string to_string(X x) {
    using Datatype = typename boost::hana::datatype<X>::type;
    return Printable::instance<Datatype>::to_string_impl(x);
}

template <>
struct Printable::instance<UserDefinedDatatype> {
    template <typename X>
    static void print_impl(std::ostream& os, X x) {
        /* a custom implementation for some data type */;
    }

    template <typename X>
    static std::string to_string_impl(X x) {
        return "a custom implementation for some data type";
    }
};
//! [main]

int main() {
    // just make sure it works
    print(std::cout, UserDefinedDatatype{});
    to_string(UserDefinedDatatype{});
}
