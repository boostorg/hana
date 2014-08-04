/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <ostream>
#include <sstream>
#include <string>
#include <utility>
using namespace boost::hana;


auto print = [](std::ostream& os, auto x) {
    // not required for the example
};

//! [Printable]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);

    struct mcd {
        template <typename X>
        static std::string to_string_impl(X x) {
            std::ostringstream os;
            print(os, x);
            return os.str();
        }
    };
};

template <typename T>
struct Printable::instance<T, when_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> : Printable::mcd {
    static void print_impl(std::ostream& os, T x)
    { os << x; }
};
//! [Printable]


int main() {
    (void)print;
}
