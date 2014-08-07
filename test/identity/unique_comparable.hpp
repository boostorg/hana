/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_IDENTITY_UNIQUE_COMPARABLE_HPP
#define BOOST_HANA_TEST_IDENTITY_UNIQUE_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/constexpr.hpp>


struct UniqueComparable { };

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<UniqueComparable, UniqueComparable>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X, Y)
        { return false_; }

        template <typename X>
        static constexpr auto equal_impl(X, X)
        { return true_; }
    };
}}

template <typename>
struct unique_impl { using hana_datatype = UniqueComparable; };

BOOST_HANA_CONSTEXPR_LAMBDA auto unique_comparable = [](auto unique) {
    return unique_impl<decltype(unique)>{};
};

#endif //! BOOST_HANA_TEST_IDENTITY_UNIQUE_COMPARABLE_HPP
