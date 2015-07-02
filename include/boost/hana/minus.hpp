/*!
@file
Defines `boost::hana::minus`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MINUS_HPP
#define BOOST_HANA_MINUS_HPP

#include <boost/hana/fwd/minus.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/fwd/negate.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/value.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) minus_t::operator()(X&& x, Y&& y) const {
        using T = typename datatype<X>::type;
        using U = typename datatype<Y>::type;
        using Minus = BOOST_HANA_DISPATCH_IF(decltype(minus_impl<T, U>{}),
            _models<Group, T>::value &&
            _models<Group, U>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Group, T>::value,
        "hana::minus(x, y) requires 'x' to be in a Group");

        static_assert(_models<Group, U>::value,
        "hana::minus(x, y) requires 'y' to be in a Group");
    #endif

        return Minus::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
    }
    //! @endcond

    template <typename T, typename U, bool condition>
    struct minus_impl<T, U, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
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
        detail::has_nontrivial_common_embedding<Group, T, U>::value
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::minus(hana::to<C>(static_cast<X&&>(x)),
                               hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct minus_impl<T, T, when<std::is_arithmetic<T>::value &&
                                 !std::is_same<bool, T>::value>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) - static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Group
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct minus_impl<C, C, when<
        _models<Constant, C>::value &&
        _models<Group, typename C::value_type>::value
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

#endif // !BOOST_HANA_MINUS_HPP
