/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_BUILTINS_INTEGER_HPP
#define BOOST_HANA_TEST_BUILTINS_INTEGER_HPP

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
};

struct integer2 {
    int value;
    constexpr explicit integer2(int i) : value{i} { }
};


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

#endif //! BOOST_HANA_TEST_BUILTINS_INTEGER_HPP
