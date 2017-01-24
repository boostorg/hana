// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/bool.hpp>
#include <boost/hana/set.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct NoDefault {
    NoDefault() = delete;
    NoDefault(NoDefault const&) = default;
    constexpr explicit NoDefault(int) { }
};

auto operator==(NoDefault const&, NoDefault const&) { return hana::true_c; }
auto operator!=(NoDefault const&, NoDefault const&) { return hana::false_c; }

struct Default {
    Default() = default;
    Default(Default const&) = default;
    constexpr explicit Default(int) { }
};

auto operator==(Default const&, Default const&) { return hana::true_c; }
auto operator!=(Default const&, Default const&) { return hana::false_c; }

int main() {
    {
        auto set0 = hana::make_set();
        static_assert(std::is_default_constructible<decltype(set0)>{}, "");

        auto set1 = hana::make_set(Default(1));
        static_assert(std::is_default_constructible<decltype(set1)>{}, "");

        auto set2 = hana::make_set(Default(1), Default(2));
        static_assert(std::is_default_constructible<decltype(set2)>{}, "");
    }

    {
        auto set1 = hana::make_set(NoDefault(1));
        static_assert(!std::is_default_constructible<decltype(set1)>{}, "");

        auto set2 = hana::make_set(NoDefault(1), NoDefault(2));
        static_assert(!std::is_default_constructible<decltype(set2)>{}, "");

        auto set3 = hana::make_set(NoDefault(1), NoDefault(2), NoDefault(3));
        static_assert(!std::is_default_constructible<decltype(set3)>{}, "");
    }
}