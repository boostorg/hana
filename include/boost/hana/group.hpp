/*!
@file
Defines `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_HPP
#define BOOST_HANA_GROUP_HPP

#include <boost/hana/fwd/group.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_common.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    namespace operators {
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
    }

    template <typename T, typename _>
    struct negate_impl<T, when<
        is_implemented<minus_impl<T, T>, _> &&
        is_implemented<zero_impl<T>, _>
    >, _> {
        template <typename X>
        static constexpr auto apply(X x)
        { return minus(zero<T>(), x); }
    };

    template <typename T, typename _>
    struct minus_impl<T, T, when<
        is_implemented<negate_impl<T>, _> &&
        is_implemented<plus_impl<T, T>, _>
    >, _> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return plus(x, negate(y)); }
    };

    template <typename T>
    struct minus_impl<T, T, when_valid<
        decltype(detail::std::declval<T>() - detail::std::declval<T>())
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return detail::std::forward<X>(x) - detail::std::forward<Y>(y);
        }
    };

    template <typename T, typename U, typename Context>
    struct dispatch_impl<4, minus_impl<T, U>, Context>
        : detail::dispatch_common<minus_impl<T, U>, Group, Context>
    { };

    template <typename G>
    constexpr auto is_a<Group, G> = bool_<
        is_a<Monoid, G>() &&
        is_implemented<negate_impl<G>> &&
        is_implemented<minus_impl<G, G>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_HPP
