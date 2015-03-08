/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/models.hpp>
#include <boost/hana/core/default.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/tuple.hpp>

#include <iostream>
#include <type_traits>
using namespace boost::hana;


struct Printable { };

template <typename T>
struct print_impl : default_ {
    /* no definition; `print(x)` fails at compile-time by default */
};

auto print = [](std::ostream& os, auto const& x) -> decltype(auto) {
    return print_impl<datatype_t<decltype(x)>>::apply(os, x);
};

namespace boost { namespace hana {
    template <typename T>
    struct models<Printable, T>
        : std::integral_constant<bool, !is_default<print_impl<T>>{}>
    { };
}}

struct Person { std::string name; };

template <>
struct print_impl<Person> {
    static std::ostream& apply(std::ostream& os, Person const& p) {
        os << p.name;
        return os;
    }
};

static_assert(models<Printable, Person>{}, "");

int main() {
    print(std::cout, Person{"Louis"});
}
