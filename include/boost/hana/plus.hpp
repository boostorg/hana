/*!
@file
Defines `boost::hana::plus`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PLUS_HPP
#define BOOST_HANA_PLUS_HPP

#include <boost/hana/fwd/plus.hpp>

#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/monoid.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) plus_t::operator()(X&& x, Y&& y) const {
        using T = typename hana::tag_of<X>::type;
        using U = typename hana::tag_of<Y>::type;
        using Plus = BOOST_HANA_DISPATCH_IF(decltype(plus_impl<T, U>{}),
            _models<Monoid, T>::value &&
            _models<Monoid, U>::value &&
            !is_default<plus_impl<T, U>>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Monoid, T>::value,
        "hana::plus(x, y) requires 'x' to be a Monoid");

        static_assert(_models<Monoid, U>::value,
        "hana::plus(x, y) requires 'y' to be a Monoid");

        static_assert(!is_default<plus_impl<T, U>>::value,
        "hana::plus(x, y) requires 'x' and 'y' to be embeddable "
        "in a common Monoid");
    #endif

        return Plus::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
    }
    //! @endcond

    template <typename T, typename U, bool condition>
    struct plus_impl<T, U, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    // Cross-type overload
    template <typename T, typename U>
    struct plus_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Monoid, T, U>::value
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::plus(hana::to<C>(static_cast<X&&>(x)),
                              hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct plus_impl<T, T, when<
        std::is_arithmetic<T>::value &&
        !std::is_same<T, bool>::value
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) + static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Monoid
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct plus_impl<C, C, when<
        _models<Constant, C>::value &&
        _models<Monoid, typename C::value_type>::value
    >> {
        using T = typename C::value_type;
        //! @cond
        template <typename X, typename Y>
        struct constant_t {
            static constexpr decltype(auto) get() {
                return boost::hana::plus(boost::hana::value<X>(),
                                         boost::hana::value<Y>());
            }

            using hana = constant_t;
            using tag = detail::CanonicalConstant<T>;
        };
        //! @endcond
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return hana::to<C>(constant_t<X, Y>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PLUS_HPP
