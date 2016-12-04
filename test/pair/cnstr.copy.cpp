// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/second.hpp>

#include <type_traits>
namespace hana = boost::hana;


template <typename Target>
struct implicit_to {
    constexpr operator Target() const { return Target{}; }
};

int main() {
    {
        typedef std::pair<int, short> P1;
        hana::pair<int, short> p1(3, 4);
        hana::pair<int, short> p2 = p1;
        BOOST_HANA_RUNTIME_CHECK(hana::first(p2) == 3);
        BOOST_HANA_RUNTIME_CHECK(hana::second(p2) == 4);
    }

    static_assert(std::is_trivially_copy_constructible<hana::pair<int, int>>{}, "");

    // make sure it also works constexpr
    {
        constexpr hana::pair<int, short> p1(3, 4);
        constexpr hana::pair<int, short> p2 = p1;
        static_assert(hana::first(p2) == 3, "");
        static_assert(hana::second(p2) == 4, "");
    }

    // Make sure it works across pair types (pair<T, U> -> pair<U, V>)
    {
        hana::pair<int, short> p1(3, 4);
        hana::pair<double, long> p2 = p1;
        BOOST_HANA_RUNTIME_CHECK(hana::first(p2) == 3);
        BOOST_HANA_RUNTIME_CHECK(hana::second(p2) == 4);
    }
    {
        struct target1 { };
        struct target2 { };
        using Target = hana::pair<target1, target2>;

        auto p1_ = hana::make_pair(target1{}, target2{});
        Target p1(p1_); (void)p1;

        auto p2_ = hana::make_pair(implicit_to<target1>{}, target2{});
        Target p2(p2_);

        auto p3_ = hana::make_pair(target1{}, implicit_to<target2>{});
        Target p3(p3_);

        auto p4_ = hana::make_pair(implicit_to<target1>{}, implicit_to<target2>{});
        Target p4(p4_);
    }

    // And also constexpr across pair types
    {
        constexpr hana::pair<int, short> p1(3, 4);
        constexpr hana::pair<double, long> p2 = p1;
        static_assert(hana::first(p2) == 3, "");
        static_assert(hana::second(p2) == 4, "");
    }
}
