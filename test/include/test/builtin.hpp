/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_BUILTIN_HPP
#define BOOST_HANA_TEST_TEST_BUILTIN_HPP

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana { namespace test {
    template <int>
    struct builtin {
        int value;
        constexpr explicit builtin(int i) : value{i} { }

        constexpr auto operator++() const { return builtin{value + 1}; }
        constexpr auto operator--() const { return builtin{value - 1}; }
        constexpr operator bool() const { return value; }
    };

    #define BOOST_HANA_DEFINE_BOOLEAN_OP(OP)                                \
        template <int i, int j>                                             \
        constexpr bool operator OP(builtin<i> x, builtin<j> y)              \
        { return x.value OP y.value; }                                      \
    /**/
    BOOST_HANA_DEFINE_BOOLEAN_OP(==)
    BOOST_HANA_DEFINE_BOOLEAN_OP(<)
    #undef BOOST_HANA_DEFINE_BOOLEAN_OP

    #define BOOST_HANA_DEFINE_OP(OP)                                        \
        template <int i, int j>                                             \
        constexpr auto operator OP(builtin<i> a, builtin<j> b)              \
        { return builtin<(i < j ? i : j)>{a.value OP b.value}; }            \
    /**/
    BOOST_HANA_DEFINE_OP(+)
    BOOST_HANA_DEFINE_OP(-)
    BOOST_HANA_DEFINE_OP(*)
    BOOST_HANA_DEFINE_OP(/)
    BOOST_HANA_DEFINE_OP(%)
    #undef BOOST_HANA_DEFINE_OP
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_BUILTIN_HPP
