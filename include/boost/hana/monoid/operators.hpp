/*!
@file
Defines custom operators for `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_OPERATORS_HPP
#define BOOST_HANA_MONOID_OPERATORS_HPP

#include <boost/hana/monoid/monoid.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana { namespace operators {
    //! Equivalent to `plus`.
    //! @relates boost::hana::Monoid
    template <typename X, typename Y, typename = detail::std::enable_if_t<
        enable_operators<Monoid, datatype_t<X>>::value ||
        enable_operators<Monoid, datatype_t<Y>>::value
    >>
    constexpr decltype(auto) operator+(X&& x, Y&& y) {
        return plus(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    }
}}} // end namespace boost::hana::operators

#endif // !BOOST_HANA_MONOID_OPERATORS_HPP
