/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_BUILTIN_INTEGER_HPP
#define BOOST_HANA_TEST_TEST_BUILTIN_INTEGER_HPP

// it is not a prerequisite of any type class, but the tests still require it
#ifndef BOOST_HANA_TEST_COMPARABLE
#   define BOOST_HANA_TEST_COMPARABLE
#endif

#ifdef BOOST_HANA_TEST_INTEGRAL_DOMAIN
#   define BOOST_HANA_TEST_RING
#endif

#ifdef BOOST_HANA_TEST_RING
#   define BOOST_HANA_TEST_GROUP
#endif

#ifdef BOOST_HANA_TEST_GROUP
#   define BOOST_HANA_TEST_MONOID
#endif

struct integer {
    int value;
    constexpr explicit integer(int i) : value{i} { }

    #ifdef BOOST_HANA_TEST_ENUMERABLE
        constexpr auto operator++() const { return integer{value + 1}; }
        constexpr auto operator--() const { return integer{value - 1}; }
    #endif
};

struct integer2 {
    int value;
    constexpr explicit integer2(int i) : value{i} { }

    #ifdef BOOST_HANA_TEST_ENUMERABLE
        constexpr auto operator++() const { return integer2{value + 1}; }
        constexpr auto operator--() const { return integer2{value - 1}; }
    #endif
};

#ifdef BOOST_HANA_TEST_COMPARABLE
    constexpr bool operator==(integer x, integer y)
    { return x.value == y.value; }

    constexpr bool operator==(integer x, integer2 y)
    { return x.value == y.value; }

    constexpr bool operator==(integer2 x, integer y)
    { return x.value == y.value; }

    constexpr bool operator==(integer2 x, integer2 y)
    { return x.value == y.value; }
#endif

#ifdef BOOST_HANA_TEST_ORDERABLE
    constexpr bool operator<(integer a, integer b)
    { return a.value < b.value; }

    constexpr bool operator<(integer2 a, integer b)
    { return a.value < b.value; }

    constexpr bool operator<(integer a, integer2 b)
    { return a.value < b.value; }

    constexpr bool operator<(integer2 a, integer2 b)
    { return a.value < b.value; }
#endif

#ifdef BOOST_HANA_TEST_MONOID
    constexpr auto operator+(integer a, integer b)
    { return integer{a.value + b.value}; }

    constexpr auto operator+(integer2 a, integer b)
    { return integer{a.value + b.value}; }

    constexpr auto operator+(integer a, integer2 b)
    { return integer{a.value + b.value}; }

    constexpr auto operator+(integer2 a, integer2 b)
    { return integer2{a.value + b.value}; }
#endif

#ifdef BOOST_HANA_TEST_GROUP
    constexpr auto operator-(integer a, integer b)
    { return integer{a.value - b.value}; }

    constexpr auto operator-(integer2 a, integer b)
    { return integer{a.value - b.value}; }

    constexpr auto operator-(integer a, integer2 b)
    { return integer{a.value - b.value}; }

    constexpr auto operator-(integer2 a, integer2 b)
    { return integer2{a.value - b.value}; }
#endif

#ifdef BOOST_HANA_TEST_RING
    constexpr auto operator*(integer a, integer b)
    { return integer{a.value * b.value}; }

    constexpr auto operator*(integer2 a, integer b)
    { return integer{a.value * b.value}; }

    constexpr auto operator*(integer a, integer2 b)
    { return integer{a.value * b.value}; }

    constexpr auto operator*(integer2 a, integer2 b)
    { return integer2{a.value * b.value}; }
#endif

#ifdef BOOST_HANA_TEST_INTEGRAL_DOMAIN
    constexpr auto operator/(integer a, integer b)
    { return integer{a.value / b.value}; }

    constexpr auto operator/(integer2 a, integer b)
    { return integer{a.value / b.value}; }

    constexpr auto operator/(integer a, integer2 b)
    { return integer{a.value / b.value}; }

    constexpr auto operator/(integer2 a, integer2 b)
    { return integer2{a.value / b.value}; }


    constexpr auto operator%(integer a, integer b)
    { return integer{a.value % b.value}; }

    constexpr auto operator%(integer2 a, integer b)
    { return integer{a.value % b.value}; }

    constexpr auto operator%(integer a, integer2 b)
    { return integer{a.value % b.value}; }

    constexpr auto operator%(integer2 a, integer2 b)
    { return integer2{a.value % b.value}; }
#endif

#endif //! BOOST_HANA_TEST_TEST_BUILTIN_INTEGER_HPP
