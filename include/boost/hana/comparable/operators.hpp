/*!
@file
Defines custom operators for `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_OPERATORS_HPP
#define BOOST_HANA_COMPARABLE_OPERATORS_HPP

#include <boost/hana/comparable/comparable.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator==(X&& x, Y&& y) {
            return equal(detail::std::forward<X>(x),
                         detail::std::forward<Y>(y));
        }

        //! Equivalent to `not_equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator!=(X&& x, Y&& y) {
            return not_equal(detail::std::forward<X>(x),
                             detail::std::forward<Y>(y));
        }
    }

    using operators::operator==;
    using operators::operator!=;
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_OPERATORS_HPP
