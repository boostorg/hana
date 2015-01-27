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
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        //! Equivalent to `mod`.
        //! @relates boost::hana::IntegralDomain
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<IntegralDomain, datatype_t<X>>::value ||
            enable_operators<IntegralDomain, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator%(X&& x, Y&& y) {
            return hana::mod(detail::std::forward<X>(x),
                             detail::std::forward<Y>(y));
        }

        //! Equivalent to `quot`.
        //! @relates boost::hana::IntegralDomain
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<IntegralDomain, datatype_t<X>>::value ||
            enable_operators<IntegralDomain, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator/(X&& x, Y&& y) {
            return hana::quot(detail::std::forward<X>(x),
                              detail::std::forward<Y>(y));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // quot
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct quot_impl : quot_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct quot_impl<T, U, when<condition>> {
        static_assert(wrong<quot_impl<T, U>>{},
        "no definition of boost::hana::quot for the given data types");
    };

    // Cross-type overload
    template <typename T, typename U>
    struct quot_impl<T, U, when<detail::has_common_embedding<IntegralDomain, T, U>{}>> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::quot(to<C>(detail::std::forward<X>(x)),
                              to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // mod
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct mod_impl : mod_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct mod_impl<T, U, when<condition>> {
        static_assert(wrong<mod_impl<T, U>>{},
        "no definition of boost::hana::mod for the given data types");
    };

    // Cross-type overload
    template <typename T, typename U>
    struct mod_impl<T, U, when<detail::has_common_embedding<IntegralDomain, T, U>{}>> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::mod(to<C>(detail::std::forward<X>(x)),
                             to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for integral data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<IntegralDomain(T), when<detail::std::is_integral<T>{}>>
        : detail::std::true_type
    { };

    template <typename T>
    struct quot_impl<T, T, when<detail::std::is_integral<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) / detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct mod_impl<T, T, when<detail::std::is_integral<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) % detail::std::forward<Y>(y); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_HPP
