/*!
@file
Defines custom operators for `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_OPERATORS_HPP
#define BOOST_HANA_GROUP_OPERATORS_HPP

#include <boost/hana/group/group.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana { namespace operators {
    //! Equivalent to `minus`.
    //! @relates boost::hana::Group
    template <typename X, typename Y, typename = detail::std::enable_if_t<
        enable_operators<Group, datatype_t<X>>::value ||
        enable_operators<Group, datatype_t<Y>>::value
    >>
    constexpr decltype(auto) operator-(X&& x, Y&& y) {
        return minus(detail::std::forward<X>(x),
                     detail::std::forward<Y>(y));
    }

    //! Equivalent to `negate`.
    //! @relates boost::hana::Group
    template <typename X, typename = detail::std::enable_if_t<
        enable_operators<Group, datatype_t<X>>::value
    >>
    constexpr decltype(auto) operator-(X&& x)
    { return negate(detail::std::forward<X>(x)); }
}}} // end namespace boost::hana::operators

#endif // !BOOST_HANA_GROUP_OPERATORS_HPP
