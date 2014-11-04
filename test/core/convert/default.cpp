/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/convert.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


template <bool b, typename ...Ts>
struct showme {
    static_assert(b, "");
};

template <typename F, typename T>
void check_convert(F f, T t) {
    using From = datatype_t<F>;
    using To = datatype_t<T>;

    // Check From -> To conversion
    BOOST_HANA_RUNTIME_ASSERT(to<To>(f) == t);
    static_assert(std::is_same<
        datatype_t<decltype(to<To>(f))>, To
    >{}, "");

    // Make sure From -> From and To -> To are the identity.
    BOOST_HANA_RUNTIME_ASSERT(to<From>(f) == f);
    static_assert(std::is_same<
        datatype_t<decltype(to<From>(f))>, From
    >{}, "");

    BOOST_HANA_RUNTIME_ASSERT(to<To>(t) == t);
    static_assert(std::is_same<
        datatype_t<decltype(to<To>(t))>, To
    >{}, "");
}

struct Datatype {
    int value;
    struct hana { using datatype = Datatype; };
    constexpr bool operator==(Datatype x)
    { return value == x.value; }
};

struct other_ctor {
    int value;
    struct hana { using datatype = Datatype; };
    constexpr bool operator==(other_ctor x)
    { return value == x.value; }
};

int main() {
    check_convert("abcdef", std::string{"abcdef"});
    check_convert(int{1}, double{1});
    check_convert(double{1}, int{1});
    check_convert(std::true_type{}, int{1});
    check_convert(std::false_type{}, int{0});
    check_convert(Datatype{1}, Datatype{1});
    check_convert(other_ctor{1}, other_ctor{1});
}
