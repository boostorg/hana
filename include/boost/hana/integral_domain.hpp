/*!
@file
Defines `boost::hana::IntegralDomain`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_DOMAIN_HPP
#define BOOST_HANA_INTEGRAL_DOMAIN_HPP

#include <boost/hana/fwd/integral_domain.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_integral.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(rem)>::value ||
            has_operator<datatype_t<Y>, decltype(rem)>::value
        >::type>
        constexpr decltype(auto) operator%(X&& x, Y&& y) {
            return hana::rem(static_cast<X&&>(x),
                             static_cast<Y&&>(y));
        }

        template <typename X, typename Y, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(quot)>::value ||
            has_operator<datatype_t<Y>, decltype(quot)>::value
        >::type>
        constexpr decltype(auto) operator/(X&& x, Y&& y) {
            return hana::quot(static_cast<X&&>(x),
                              static_cast<Y&&>(y));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // quot
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct quot_impl : quot_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct quot_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr void apply(X&&, Y&&) {
            static_assert(wrong<quot_impl<T, U>, X, Y>{},
            "no definition of boost::hana::quot for the given data types");
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct quot_impl<T, U, when<
        detail::has_nontrivial_common_embedding<IntegralDomain, T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::quot(to<C>(static_cast<X&&>(x)),
                              to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // mod
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct rem_impl : rem_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct rem_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr void apply(X&&, Y&&) {
            static_assert(wrong<rem_impl<T, U>, X, Y>{},
            "no definition of boost::hana::rem for the given data types");
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct rem_impl<T, U, when<
        detail::has_nontrivial_common_embedding<IntegralDomain, T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::rem(to<C>(static_cast<X&&>(x)),
                             to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename D>
    struct models_impl<IntegralDomain, D>
        : _integral_constant<bool,
            !is_default<rem_impl<D, D>>{} &&
            !is_default<quot_impl<D, D>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for integral data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct quot_impl<T, T, when<detail::std::is_non_boolean_integral<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) / static_cast<Y&&>(y); }
    };

    template <typename T>
    struct rem_impl<T, T, when<detail::std::is_non_boolean_integral<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) % static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over an IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct quot_impl<C, C, when<
        _models<Constant, C>{} && _models<IntegralDomain, typename C::value_type>{}
    >> {
        using T = typename C::value_type;
        template <typename X, typename Y>
        struct _constant {
            static constexpr decltype(auto) get() {
                return boost::hana::quot(boost::hana::value<X>(),
                                         boost::hana::value<Y>());
            }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return to<C>(_constant<X, Y>{}); }
    };

    template <typename C>
    struct rem_impl<C, C, when<
        _models<Constant, C>{} && _models<IntegralDomain, typename C::value_type>{}
    >> {
        using T = typename C::value_type;
        template <typename X, typename Y>
        struct _constant {
            static constexpr decltype(auto) get() {
                return boost::hana::rem(boost::hana::value<X>(),
                                        boost::hana::value<Y>());
            }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return to<C>(_constant<X, Y>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_DOMAIN_HPP
