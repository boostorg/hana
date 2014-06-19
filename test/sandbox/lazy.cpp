/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/sandbox/lazy.hpp>

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <cassert>
#include <type_traits>
using namespace boost::hana;
using sandbox::lazy;


struct anything { };

auto test = [](auto lazy, auto implicit_conversion_to_int) {
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = lazy([](auto x) { return x + 1; });
    BOOST_HANA_STATIC_ASSERT(implicit_conversion_to_int(f(1)) == 2);
    BOOST_HANA_STATIC_ASSERT(static_cast<int>(f(3)) == 4);

    using LazyResult = decltype(f(1));
    static_assert(!std::is_same<LazyResult, int>{}, "");
    static_assert(!std::is_convertible<LazyResult, anything>{}, "");
    static_assert(std::is_same<std::common_type_t<LazyResult, int>, int>{}, "");
    static_assert(std::is_same<datatype_t<LazyResult>, datatype_t<int>>{}, "");


    BOOST_HANA_CONSTEXPR_LAMBDA auto g = lazy([](auto x) { return x + 1; });
    static_assert(std::is_same<std::common_type_t<LazyResult, decltype(g(1))>, int>{}, "");


    auto invalid = lazy([](auto x) {
        throw "invalid was called";
        return x + 1;
    });
    invalid(1); // should not throw
    try {
        (void)static_cast<int>(invalid(1)); // should throw here
        assert(false); // <- this is never executed
    } catch (...) { }
};

int main() {
    test(lazy<int>, [](auto x) { return x; });

    // In the fully lazy version, it can happen that overload resolution is
    // ambiguous if there are multiple overloads for types to which the lazy
    // result is convertible. We must use an explicit cast in that case.
    test(lazy<>, [](auto x) { return static_cast<int>(x); });

    // In the fully lazy version, we also make sure that invalid expressions
    // are not eagerly instantiated.
    BOOST_HANA_CONSTEXPR_LAMBDA auto invalid = lazy<>([](auto x) {
        return x + (void)1; // would fail to compile
    });
    invalid(1); // <-- the lambda is not instantiated
}
