/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/dependent_on.hpp>

#include <cassert>
#include <ostream>
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
    return P::template instance<decltype(x)>::print_impl(os, x);
};

auto to_string = [](auto x) {
    using P = detail::dependent_on_t<(bool)sizeof(x), Printable>;
    return P::template instance<decltype(x)>::to_string_impl(x);
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
//! [Printable]

//! [instances]
template <typename T>
struct Printable::instance<T, when_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> : Printable::mcd {
    static void print_impl(std::ostream& os, T x)
    { os << x; }
};

template <typename T>
struct Printable::instance<std::vector<T>, when_valid<
    decltype(std::declval<std::ostream&>() << std::declval<T>())
>> : Printable::mcd {
    static void print_impl(std::ostream& os, std::vector<T> v) {
        os << '[';
        for (auto it = begin(v); it != end(v); ) {
            os << *it;
            if (++it != end(v))
                os << ", ";
        }
        os << ']';
    }
};
//! [instances]


int main() {
    std::vector<int> v{1, 2, 3};

    std::stringstream ss;
    print(ss, v);
    assert(ss.str() == "[1, 2, 3]");

    assert(to_string(v) == "[1, 2, 3]");
}
