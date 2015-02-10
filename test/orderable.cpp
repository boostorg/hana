/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/auto/orderable.hpp>

#include <type_traits>
using namespace boost::hana;


// Minimal LessThanComparable types
struct ord1 { int value; };
struct ord2 {
    int value;
    constexpr operator ord1() const { return {value}; }
};

template <typename T, typename U, typename = std::enable_if_t<
    (std::is_same<T, ord1>{} || std::is_same<T, ord2>{}) &&
    (std::is_same<U, ord1>{} || std::is_same<U, ord2>{})
>>
constexpr bool operator<(T a, U b)
{ return a.value < b.value; }


namespace boost { namespace hana { namespace test {
    template <> auto objects<int> = tuple(0,1,2,3,4,5);
    template <> auto objects<unsigned int> = tuple(0u,1u,2u,3u,4u,5u);
    template <> auto objects<long> = tuple(0l,1l,2l,3l,4l,5l);
    template <> auto objects<unsigned long> = tuple(0ul,1ul,2ul,3ul,4ul,5ul);
    template <> auto objects<ord1> = tuple(ord1{0}, ord1{1}, ord1{2}, ord1{3}, ord1{4});
}}}


int main() {
    // laws
    test::laws<Orderable, int>();
    test::laws<Orderable, unsigned int>();
    test::laws<Orderable, long>();
    test::laws<Orderable, unsigned long>();
    test::laws<Orderable, ord1>();

    // less
    {
        BOOST_HANA_CONSTEXPR_CHECK(less(5, 6));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(6, 6)));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(7, 6)));

        // Provided model for LessThanComparable types
        BOOST_HANA_CONSTEXPR_CHECK(less(ord1{0}, ord1{1}));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord1{0}, ord1{0})));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord1{1}, ord1{0})));

        BOOST_HANA_CONSTEXPR_CHECK(less(ord1{0}, ord2{1}));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord1{0}, ord2{0})));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord1{1}, ord2{0})));

        BOOST_HANA_CONSTEXPR_CHECK(less(ord2{0}, ord1{1}));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord2{0}, ord1{0})));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less(ord2{1}, ord1{0})));
    }

    // greater
    {
        BOOST_HANA_CONSTEXPR_CHECK(not_(greater(5, 6)));
        BOOST_HANA_CONSTEXPR_CHECK(not_(greater(6, 6)));
        BOOST_HANA_CONSTEXPR_CHECK(greater(7, 6));
    }

    // less_equal
    {
        BOOST_HANA_CONSTEXPR_CHECK(less_equal(5, 6));
        BOOST_HANA_CONSTEXPR_CHECK(less_equal(6, 6));
        BOOST_HANA_CONSTEXPR_CHECK(not_(less_equal(7, 6)));
    }

    // greater_equal
    {
        BOOST_HANA_CONSTEXPR_CHECK(not_(greater_equal(5, 6)));
        BOOST_HANA_CONSTEXPR_CHECK(greater_equal(6, 6));
        BOOST_HANA_CONSTEXPR_CHECK(greater_equal(7, 6));
    }

    // min
    {
        BOOST_HANA_CONSTEXPR_CHECK(equal(min(5, 6), 5));
        BOOST_HANA_CONSTEXPR_CHECK(equal(min(6, 5), 5));
    }

    // max
    {
        BOOST_HANA_CONSTEXPR_CHECK(equal(max(5, 6), 6));
        BOOST_HANA_CONSTEXPR_CHECK(equal(max(6, 5), 6));
    }
}
