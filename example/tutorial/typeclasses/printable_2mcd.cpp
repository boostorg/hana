/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/dependent_on.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace boost::hana;


struct Printable;

// We use `detail::dependent_on_t` to make sure we don't fetch
// `Printable::instance` until it is defined below. A small hack
// to make the example simpler is worth it!
auto print = [](std::ostream& os, auto x) {
    using P = detail::dependent_on_t<(bool)sizeof(x), Printable>;
    return P::template instance<
        datatype_t<decltype(x)>
    >::print_impl(os, x);
};

auto to_string = [](auto x) {
    using P = detail::dependent_on_t<(bool)sizeof(x), Printable>;
    return P::template instance<
        datatype_t<decltype(x)>
    >::to_string_impl(x);
};

//! [Printable]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);

    // requires to_string only
    struct to_string_mcd {
        template <typename X>
        static void print_impl(std::ostream& os, X x) {
            os << to_string(x);
        }
    };

    // requires print only
    struct print_mcd {
        template <typename X>
        static std::string to_string_impl(X x) {
            std::ostringstream os;
            print(os, x);
            return os.str();
        }
    };
};
//! [Printable]

template <typename T>
struct Printable::instance<T, when<is_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>>> : Printable::print_mcd {
    static void print_impl(std::ostream& os, T x)
    { os << x; }
};

//! [string_instance]
template <>
struct Printable::instance<std::string>
    : Printable::print_mcd // could also be to_string_mcd, it doesn't matter
{
    static void print_impl(std::ostream& os, std::string x)
    { os << x; }

    static std::string to_string_impl(std::string x)
    { return x; }
};
//! [string_instance]


//! [vector_instance]
template <typename T>
struct Printable::instance<std::vector<T>, when<is_a<Printable, T>()>>
    : Printable::print_mcd
{
    static void print_impl(std::ostream& os, std::vector<T> v) {
        os << '[';
        for (auto it = begin(v); it != end(v); ) {
            print(os, *it); // recursively print the contents
            if (++it != end(v))
                os << ", ";
        }
        os << ']';
    }
};
//! [vector_instance]


int main() {
    //! [print_vector]
std::vector<std::vector<int>> v{{1, 2, 3}, {3, 4, 5}};
print(std::cout, v); // prints "[[1, 2, 3], [3, 4, 5]]"
    //! [print_vector]

    std::stringstream ss;
    print(ss, v);
    BOOST_HANA_RUNTIME_ASSERT(ss.str() == "[[1, 2, 3], [3, 4, 5]]");

    BOOST_HANA_RUNTIME_ASSERT(to_string(v) == "[[1, 2, 3], [3, 4, 5]]");
}
