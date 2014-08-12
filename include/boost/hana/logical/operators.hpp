/*!
@file
Defines custom operators for `boost::hana::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_OPERATORS_HPP
#define BOOST_HANA_LOGICAL_OPERATORS_HPP

#include <boost/hana/logical/logical.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `and_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator&&(X&& x, Y&& y) {
            return and_(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        //! Equivalent to `or_`.
        //! @relates boost::hana::Logical
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Logical, datatype_t<X>>::value ||
            enable_operators<Logical, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator||(X&& x, Y&& y) {
            return or_(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        //! Equivalent to `not_`.
        //! @relates boost::hana::Logical
        template <typename X, typename = detail::std::enable_if_t<
            enable_operators<Logical, datatype_t<X>>::value
        >>
        constexpr decltype(auto) operator!(X&& x) {
            return not_(detail::std::forward<X>(x));
        }
    }

    using operators::operator!;
    using operators::operator&&;
    using operators::operator||;
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_OPERATORS_HPP
