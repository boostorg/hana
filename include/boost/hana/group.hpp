/*!
@file
Defines `boost::hana::Group`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_HPP
#define BOOST_HANA_GROUP_HPP

#include <boost/hana/fwd/group.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/detail/wrong.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(minus)>{}() ||
            _has_operator<datatype_t<Y>, decltype(minus)>{}()
        >>
        constexpr decltype(auto) operator-(X&& x, Y&& y)
        { return hana::minus(static_cast<X&&>(x), static_cast<Y&&>(y)); }

        template <typename X, typename = detail::std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(negate)>{}()
        >>
        constexpr decltype(auto) operator-(X&& x)
        { return hana::negate(static_cast<X&&>(x)); }
    }

    //////////////////////////////////////////////////////////////////////////
    // minus
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct minus_impl : minus_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct minus_impl<T, U, when<condition>> : default_ {
        static void apply(...);
    };

    template <typename T, bool condition>
    struct minus_impl<T, T, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::plus(static_cast<X&&>(x),
                              hana::negate(static_cast<Y&&>(y)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct minus_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Group, T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::minus(hana::to<C>(static_cast<X&&>(x)),
                               hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // negate
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct negate_impl : negate_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct negate_impl<T, when<condition>> : default_ {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::minus(zero<T>(), static_cast<X&&>(x)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename G>
    struct models_impl<Group, G>
        : _integral_constant<bool,
            !is_default<negate_impl<G>>{}() ||
            !is_default<minus_impl<G, G>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct minus_impl<T, T, when<detail::std::is_non_boolean_arithmetic<T>{}()>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) - static_cast<Y&&>(y); }
    };

    template <typename T>
    struct negate_impl<T, when<detail::std::is_non_boolean_arithmetic<T>{}()>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return -static_cast<X&&>(x); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Group
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct minus_impl<C, C, when<
        _models<Constant, C>{}() &&
        _models<Group, typename C::value_type>{}()
    >> {
        using T = typename C::value_type;
        template <typename X, typename Y>
        struct _constant {
            static constexpr decltype(auto) get() {
                return boost::hana::minus(boost::hana::value<X>(),
                                          boost::hana::value<Y>());
            }

            using hana = _constant;
            using datatype = detail::CanonicalConstant<T>;
        };
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return hana::to<C>(_constant<X, Y>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_HPP
