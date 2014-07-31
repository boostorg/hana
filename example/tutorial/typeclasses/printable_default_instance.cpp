/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/dependent_on.hpp>

#include <iostream>
#include <string>
#include <typeinfo>
using namespace boost::hana;


struct Printable;

// We use `detail::dependent_on_t` to make sure we don't fetch
// `Printable::instance` until it is defined below. A small hack
// to make the example simpler is worth it!
auto print = [](std::ostream& os, auto x) {
    using P = detail::dependent_on_t<(bool)sizeof(x), Printable>;
    return P::template instance<decltype(x)>::print_impl(os, x);
};

auto to_string = [](auto x) {
    using P = detail::dependent_on_t<(bool)sizeof(x), Printable>;
    return P::template instance<decltype(x)>::to_string_impl(x);
};

//! [Printable]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);

    // definitions omitted
    struct to_string_mcd { };
    struct print_mcd { };

    template <typename T>
    struct default_instance : print_mcd {
        static void print_impl(std::ostream& os, T)
        { os << "<" << typeid(T).name() << ">"; }
    };
};
//! [Printable]


int main() {
    //! [print_foo]
struct foo { };
// prints some implementation-defined stuff representing foo
print(std::cout, foo{});
    //! [print_foo]

    (void)to_string; // remove unused variable warning
}
