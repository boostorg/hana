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

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `quot` and `mod`
    struct IntegralDomain::mcd { };

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

    template <typename T, typename U>
    struct IntegralDomain::default_instance
        : IntegralDomain::instance<common_t<T, U>, common_t<T, U>>
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) quot_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return quot(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) mod_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return mod(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };

    //! Instance of `IntegralDomain` for objects of foreign numeric types.
    //!
    //! Any two foreign objects that are `Rings`s, that can be divided
    //! and moded with the usual operators (`/` and `%`) naturally form
    //! an integral domain with those operations.
    template <typename T, typename U>
    struct IntegralDomain::instance<T, U, when_valid<
        decltype(detail::std::declval<T>() / detail::std::declval<U>()),
        decltype(detail::std::declval<T>() % detail::std::declval<U>()),
        char[are<Ring, T, U>()]
    >> : IntegralDomain::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) quot_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) / detail::std::forward<Y>(y);
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) mod_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) % detail::std::forward<Y>(y);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_HPP
