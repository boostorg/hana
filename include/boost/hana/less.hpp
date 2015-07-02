/*!
@file
Defines `boost::hana::less`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LESS_HPP
#define BOOST_HANA_LESS_HPP

#include <boost/hana/fwd/less.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/concepts.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/nested_than.hpp> // required by fwd decl
#include <boost/hana/if.hpp>
#include <boost/hana/value.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename X, typename Y>
    constexpr auto less_t::operator()(X&& x, Y&& y) const {
        using T = typename hana::datatype<X>::type;
        using U = typename hana::datatype<Y>::type;
        using Less = BOOST_HANA_DISPATCH_IF(decltype(less_impl<T, U>{}),
            _models<Orderable, T>::value &&
            _models<Orderable, U>::value &&
            !is_default<less_impl<T, U>>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Orderable, T>::value,
        "hana::less(x, y) requires 'x' to be Orderable");

        static_assert(_models<Orderable, U>::value,
        "hana::less(x, y) requires 'y' to be Orderable");

        static_assert(!is_default<less_impl<T, U>>::value,
        "hana::less(x, y) requires 'x' and 'y' to be embeddable "
        "in a common Orderable");
    #endif

        return Less::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
    }
    //! @endcond

    template <typename T, typename U, bool condition>
    struct less_impl<T, U, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    // Cross-type overload
    template <typename T, typename U>
    struct less_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>::value &&
        !detail::LessThanComparable<T, U>::value
    >> {
        using C = typename hana::common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less(hana::to<C>(static_cast<X&&>(x)),
                              hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for LessThanComparable data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct less_impl<T, U, when<detail::LessThanComparable<T, U>::value>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) < static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants wrapping an Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct less_impl<C, C, when<
        _models<Constant, C>::value &&
        _models<Orderable, typename C::value_type>::value
    >> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&) {
            constexpr auto less = hana::less(hana::value<X>(), hana::value<Y>());
            constexpr bool truth_value = hana::if_(less, true, false);
            return hana::bool_<truth_value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LESS_HPP
