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


template <typename X, typename Y>
constexpr auto operator==(X x, Y y)
{ return x.value == y.value; }

struct Datatype {
    int value;
    struct hana { using datatype = Datatype; };
};

struct Other {
    int value;
    struct hana { using datatype = Datatype; };
};

struct SpecializedFrom;
struct specialized_from {
    int value;
    struct hana { using datatype = SpecializedFrom; };
};

struct SpecializedTo;
struct specialized_to {
    int value;
    struct hana { using datatype = SpecializedTo; };
};

namespace boost { namespace hana {
    template <>
    struct to_impl<SpecializedTo, SpecializedFrom> {
        template <typename T>
        static constexpr auto apply(T t)
        { return specialized_to{t.value}; }
    };
}}

template <typename F, typename T>
void check_convert(F f, T t) {
    using From = datatype_t<F>;
    using To = datatype_t<T>;

    // Check From -> To conversion
    BOOST_HANA_RUNTIME_CHECK(to<To>(f) == t);
    static_assert(std::is_same<
        datatype_t<decltype(to<To>(f))>, To
    >{}, "");

    static_assert(is_convertible<From, To>{}, "");


    // Make sure From -> From and To -> To are the identity.
    BOOST_HANA_RUNTIME_CHECK(to<From>(f) == f);
    static_assert(std::is_same<
        datatype_t<decltype(to<From>(f))>, From
    >{}, "");

    BOOST_HANA_RUNTIME_CHECK(to<To>(t) == t);
    static_assert(std::is_same<
        datatype_t<decltype(to<To>(t))>, To
    >{}, "");

    static_assert(is_convertible<From, From>{}, "");
    static_assert(is_convertible<To, To>{}, "");

    static_assert(is_embedded<From, From>{}, "");
    static_assert(is_embedded<To, To>{}, "");
}

int main() {
    check_convert("abcdef", std::string{"abcdef"});
    check_convert(int{1}, double{1});
    check_convert(double{1}, int{1});
    check_convert(std::true_type{}, int{1});
    check_convert(std::false_type{}, int{0});
    check_convert(Datatype{1}, Datatype{1});
    check_convert(Other{1}, Other{1});
    check_convert(specialized_from{1}, specialized_to{1});

    static_assert(!is_convertible<void, int>{}, "");
    static_assert(!is_embedded<void, int>{}, "");

    static_assert(is_convertible<int, void>{}, "");
    static_assert(!is_embedded<int, void>{}, "");
}
