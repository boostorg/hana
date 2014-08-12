/*!
@file
Defines the instance of `boost::hana::Comparable` for `boost::hana::Bool`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_COMPARABLE_HPP
#define BOOST_HANA_BOOL_COMPARABLE_HPP

#include <boost/hana/bool/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<Bool, Bool> : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return bool_<x() == y()>; }
    };
}}

#endif // !BOOST_HANA_BOOL_COMPARABLE_HPP
