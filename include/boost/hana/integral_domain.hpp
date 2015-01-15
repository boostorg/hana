/*!
@file
Defines `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_HPP

#include <boost/hana/fwd/integral_domain.hpp>

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
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `mod`.
        //! @relates boost::hana::IntegralDomain
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<IntegralDomain, datatype_t<X>>::value ||
            enable_operators<IntegralDomain, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator%(X&& x, Y&& y)
        { return mod(detail::std::forward<X>(x), detail::std::forward<Y>(y)); }

        //! Equivalent to `quot`.
        //! @relates boost::hana::IntegralDomain
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<IntegralDomain, datatype_t<X>>::value ||
            enable_operators<IntegralDomain, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator/(X&& x, Y&& y)
        { return quot(detail::std::forward<X>(x), detail::std::forward<Y>(y)); }
    }

    BOOST_HANA_DISPATCH_COMMON(quot, quot_impl, IntegralDomain);
    BOOST_HANA_DISPATCH_COMMON(mod, mod_impl, IntegralDomain);

    template <typename T>
    struct quot_impl<T, T, when_valid<
        decltype(detail::std::declval<T>() / detail::std::declval<T>())
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) / detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct mod_impl<T, T, when_valid<
        decltype(detail::std::declval<T>() % detail::std::declval<T>())
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) % detail::std::forward<Y>(y); }
    };

    template <typename D>
    constexpr auto is_a<IntegralDomain, D> = bool_<
        is_a<Ring, D>() &&
        is_implemented<quot_impl<D, D>> &&
        is_implemented<mod_impl<D, D>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_HPP
