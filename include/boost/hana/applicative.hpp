/*!
@file
Defines `boost::hana::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_HPP
#define BOOST_HANA_APPLICATIVE_HPP

#include <boost/hana/fwd/applicative.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // ap
    //////////////////////////////////////////////////////////////////////////
    template <typename A, typename>
    struct ap_impl : ap_impl<A, when<true>> { };

    template <typename A, bool condition>
    struct ap_impl<A, when<condition>> {
        static_assert(wrong<ap_impl<A>>{},
        "no definition of boost::hana::ap for the given data type");
    };

    template <typename F, typename X>
    constexpr decltype(auto) _ap::operator()(F&& f, X&& x) const {
        return ap_impl<typename datatype<F>::type>::apply(
            detail::std::forward<F>(f), detail::std::forward<X>(x)
        );
    }

    template <typename F, typename ...Xs>
    constexpr decltype(auto) _ap::operator()(F&& f, Xs&& ...xs) const {
        static_assert(sizeof...(xs) >= 1,
        "boost::hana::ap must be called with at least two arguments");
        return detail::variadic::foldl(
            *this,
            hana::fmap(detail::std::forward<F>(f), curry<sizeof...(xs)>),
            detail::std::forward<Xs>(xs)...
        );
    }

    //////////////////////////////////////////////////////////////////////////
    // lift
    //////////////////////////////////////////////////////////////////////////
    template <typename A, typename>
    struct lift_impl : lift_impl<A, when<true>> { };

    template <typename A, bool condition>
    struct lift_impl<A, when<condition>> {
        static_assert(wrong<lift_impl<A>>{},
        "no definition of boost::hana::lift for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative::fmap_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename A>
    struct Applicative::fmap_impl {
        template <typename X, typename F>
        static constexpr decltype(auto) apply(X&& x, F&& f) {
            return hana::ap(lift<A>(detail::std::forward<F>(f)),
                            detail::std::forward<X>(x));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_HPP
