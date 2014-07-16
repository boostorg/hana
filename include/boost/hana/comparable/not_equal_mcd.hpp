/*!
@file
Defines `boost::hana::Comparable::not_equal_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_NOT_EQUAL_MCD_HPP
#define BOOST_HANA_COMPARABLE_NOT_EQUAL_MCD_HPP

#include <boost/hana/comparable/comparable.hpp>

#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `not_equal`
    struct Comparable::not_equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return not_(not_equal(x, y)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_NOT_EQUAL_MCD_HPP
