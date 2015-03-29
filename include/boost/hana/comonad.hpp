/*!
@file
Defines `boost::hana::Comonad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMONAD_HPP
#define BOOST_HANA_COMONAD_HPP

#include <boost/hana/fwd/comonad.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // extract
    //////////////////////////////////////////////////////////////////////////
    template <typename W, typename>
    struct extract_impl : extract_impl<W, when<true>> { };

    template <typename W, bool condition>
    struct extract_impl<W, when<condition>> : default_ {
        static void apply(...);
    };

    //////////////////////////////////////////////////////////////////////////
    // duplicate
    //////////////////////////////////////////////////////////////////////////
    template <typename W, typename>
    struct duplicate_impl : duplicate_impl<W, when<true>> { };

    template <typename W, bool condition>
    struct duplicate_impl<W, when<condition>> : default_ {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::extend(static_cast<X&&>(x), hana::id); }
    };

    //////////////////////////////////////////////////////////////////////////
    // extend
    //////////////////////////////////////////////////////////////////////////
    template <typename W, typename>
    struct extend_impl : extend_impl<W, when<true>> { };

    template <typename W, bool condition>
    struct extend_impl<W, when<condition>> : default_ {
        template <typename X, typename F>
        static constexpr decltype(auto) apply(X&& x, F&& f) {
            return hana::transform(hana::duplicate(static_cast<X&&>(x)),
                                   static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename W>
    struct models_impl<Comonad, W>
        : _integral_constant<bool,
            !is_default<extract_impl<W>>{} &&
            (!is_default<duplicate_impl<W>>{} || !is_default<extend_impl<W>>{})
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMONAD_HPP
