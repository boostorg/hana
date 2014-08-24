/*!
@file
Defines custom operators for `boost::hana::Orderable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_OPERATORS_HPP
#define BOOST_HANA_ORDERABLE_OPERATORS_HPP

#include <boost/hana/orderable/orderable.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana { namespace operators {
    //! Equivalent to `less`.
    //! @relates boost::hana::Orderable
    template <typename X, typename Y, typename = detail::std::enable_if_t<
        enable_operators<Orderable, datatype_t<X>>::value ||
        enable_operators<Orderable, datatype_t<Y>>::value
    >>
    constexpr decltype(auto) operator<(X&& x, Y&& y) {
        return less(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    }

    //! Equivalent to `less_equal`.
    //! @relates boost::hana::Orderable
    template <typename X, typename Y, typename = detail::std::enable_if_t<
        enable_operators<Orderable, datatype_t<X>>::value ||
        enable_operators<Orderable, datatype_t<Y>>::value
    >>
    constexpr decltype(auto) operator<=(X&& x, Y&& y) {
        return less_equal(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    }

    //! Equivalent to `greater`.
    //! @relates boost::hana::Orderable
    template <typename X, typename Y, typename = detail::std::enable_if_t<
        enable_operators<Orderable, datatype_t<X>>::value ||
        enable_operators<Orderable, datatype_t<Y>>::value
    >>
    constexpr decltype(auto) operator>(X&& x, Y&& y) {
        return greater(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    }

    //! Equivalent to `greater_equal`.
    //! @relates boost::hana::Orderable
    template <typename X, typename Y, typename = detail::std::enable_if_t<
        enable_operators<Orderable, datatype_t<X>>::value ||
        enable_operators<Orderable, datatype_t<Y>>::value
    >>
    constexpr decltype(auto) operator>=(X&& x, Y&& y) {
        return greater_equal(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    }
}}} // end namespace boost::hana::operators

#endif // !BOOST_HANA_ORDERABLE_OPERATORS_HPP
