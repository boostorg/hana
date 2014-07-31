/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <cassert>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace boost::hana;


//! [recap]
struct Printable {
    BOOST_HANA_TYPECLASS(Printable);
};

auto print = [](std::ostream& os, auto x) {
    return Printable::instance<decltype(x)>::print_impl(os, x);
};

auto to_string = [](auto x) {
    return Printable::instance<decltype(x)>::to_string_impl(x);
};

template <typename T>
struct Printable::instance<T, when_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> {
    static void print_impl(std::ostream& os, T x)
    { os << x; }

    static std::string to_string_impl(T x) {
        std::ostringstream os;
        print(os, x);
        return os.str();
    }
};

template <>
struct Printable::instance<std::string> {
    static void print_impl(std::ostream& os, std::string x)
    { os << x; }

    static std::string to_string_impl(std::string x)
    { return x; }
};
//! [recap]

//! [vector_streamable_instance]
template <typename T>
struct Printable::instance<std::vector<T>, when_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> {
    static void print_impl(std::ostream& os, std::vector<T> v) {
        os << '[';
        for (auto it = begin(v); it != end(v); ) {
            os << *it;
            if (++it != end(v))
                os << ", ";
        }
        os << ']';
    }

    static std::string to_string_impl(std::vector<T> v) {
        std::ostringstream os;
        print(os, v);
        return os.str();
    }
};
//! [vector_streamable_instance]


int main() {
    std::vector<int> v{1, 2, 3};

    std::stringstream ss;
    print(ss, v);
    assert(ss.str() == "[1, 2, 3]");

    assert(to_string(v) == "[1, 2, 3]");
}
