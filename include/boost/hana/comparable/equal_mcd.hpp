/*!
@file
Defines `boost::hana::Comparable::equal_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_EQUAL_MCD_HPP
#define BOOST_HANA_COMPARABLE_EQUAL_MCD_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `equal`
    struct Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return not_(equal(x, y)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_EQUAL_MCD_HPP

#include <boost/hana/comparable/default.hpp> //! @todo remove this include
