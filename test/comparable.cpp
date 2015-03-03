/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>

#include <type_traits>
using namespace boost::hana;


// Minimal EqualityComparable types
struct eq1 { int value; };
struct eq2 {
    int value;
    constexpr operator eq1() const { return {value}; }
};

template <typename T, typename U, typename = std::enable_if_t<
    (std::is_same<T, eq1>{} || std::is_same<T, eq2>{}) &&
    (std::is_same<U, eq1>{} || std::is_same<U, eq2>{})
>>
constexpr bool operator==(T a, U b)
{ return a.value == b.value; }

template <typename T, typename U, typename = std::enable_if_t<
    (std::is_same<T, eq1>{} || std::is_same<T, eq2>{}) &&
    (std::is_same<U, eq1>{} || std::is_same<U, eq2>{})
>>
constexpr bool operator!=(T a, U b)
{ return !(a == b); }


namespace boost { namespace hana { namespace test {
    template <> auto objects<int> = make<Tuple>(0,1,2,3,4,5);
    template <> auto objects<unsigned int> = make<Tuple>(0u,1u,2u,3u,4u,5u);
    template <> auto objects<long> = make<Tuple>(0l,1l,2l,3l,4l,5l);
    template <> auto objects<unsigned long> = make<Tuple>(0ul,1ul,2ul,3ul,4ul,5ul);
    template <> auto objects<eq1> = make<Tuple>(eq1{0}, eq1{1}, eq1{2}, eq1{3}, eq1{4});
}}}

int main() {
    // laws
    test::laws<Orderable, int>();
    test::laws<Orderable, unsigned int>();
    test::laws<Orderable, long>();
    test::laws<Orderable, unsigned long>();
    test::laws<Comparable, eq1>();

    // equal
    {
        // Two objects of different data types are unequal by default,
        // and no model is provided for two objects of the same data type.
        struct Random1 { }; struct Random2 { };
        BOOST_HANA_CONSTANT_CHECK(not_(equal(Random1{}, Random2{})));
        static_assert(!models<Comparable(Random1)>{}, "");
        static_assert(!models<Comparable(Random2)>{}, "");

        // Provided model for EqualityComparable types
        BOOST_HANA_CONSTEXPR_CHECK(equal(eq1{0}, eq1{0}));
        BOOST_HANA_CONSTEXPR_CHECK(not_(equal(eq1{0}, eq1{1})));
        BOOST_HANA_CONSTEXPR_CHECK(not_(equal(eq1{1}, eq1{0})));

        BOOST_HANA_CONSTEXPR_CHECK(equal(eq1{0}, eq2{0}));
        BOOST_HANA_CONSTEXPR_CHECK(not_(equal(eq1{0}, eq2{1})));
        BOOST_HANA_CONSTEXPR_CHECK(not_(equal(eq1{1}, eq2{0})));

        BOOST_HANA_CONSTEXPR_CHECK(equal(eq2{0}, eq1{0}));
        BOOST_HANA_CONSTEXPR_CHECK(not_(equal(eq2{0}, eq1{1})));
        BOOST_HANA_CONSTEXPR_CHECK(not_(equal(eq2{1}, eq1{0})));
    }
}
