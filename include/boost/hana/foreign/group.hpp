/*!
@file
Defines the instance of `boost::hana::Group` for `boost::hana::Foreign`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_GROUP_HPP
#define BOOST_HANA_FOREIGN_GROUP_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foreign/foreign.hpp>
#include <boost/hana/group/minus_mcd.hpp>

// Mcd
#include <boost/hana/foreign/monoid.hpp>


namespace boost { namespace hana {
    //! Instance of `Group` for `Foreign` objects with numeric types.
    //!
    //! Any two `Foreign` objects forming a `Monoid` and that can be
    //! subtracted with the usual `operator-` naturally form an additive
    //! group, with the group subtraction being that usual `operator-`.
    template <typename T, typename U>
    struct Group::instance<Foreign<T>, Foreign<U>, when<
        are<Monoid, Foreign<T>, Foreign<U>>() &&
        is_valid<decltype((void)(*(T*)0 - *(U*)0))>
    >>
        : Group::minus_mcd<Foreign<T>, Foreign<U>>
    {
        template <typename X, typename Y>
        static constexpr auto minus_impl(X x, Y y)
        { return x - y; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_GROUP_HPP
