/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
using namespace boost::hana;


struct Printable {
    BOOST_HANA_TYPECLASS(Printable);
};

auto print = [](std::ostream& os, auto x) {
    return Printable::instance<decltype(x)>::print_impl(os, x);
};

//! [streamable_instance]
template <typename T>
struct Printable::instance<T, when_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> {
    static void print_impl(std::ostream& os, T x) {
        os << x;
    }
};
//! [streamable_instance]

void counter_example() {
    //! [to_string_non_method]
auto to_string = [](auto x) {
    std::ostringstream os;
    print(os, x);
    return os.str();
};

to_string(1);
    //! [to_string_non_method]
}


int main() {
    //! [print_string]
print(std::cout, std::string{"foo"});
    //! [print_string]
}
