/*!
@file
Defines `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_HPP
#define BOOST_HANA_LAZY_HPP

#include <boost/hana/fwd/lazy.hpp>

#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/compose.hpp>

// instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    //! Instance of `Applicative` for `Lazy`.
    //!
    //! A normal value can be lifted into a lazy value by using `lift<Lazy>`.
    //! A lazy function can be lazily applied to a lazy value by using `ap`.
    template <>
    struct Applicative::instance<Lazy> : Applicative::mcd {
        static constexpr auto lift_impl = lazy;

        template <typename Lf, typename Lx>
        struct ap_result {
            Lf lf; Lx lx;
            struct hana { using datatype = Lazy; };

            template <typename Id>
            constexpr decltype(auto) eval_impl(Id _) const&
            { return _(eval)(lf)(_(eval)(lx)); }

            template <typename Id>
            constexpr decltype(auto) eval_impl(Id _) &
            { return _(eval)(lf)(_(eval)(lx)); }

            template <typename Id>
            constexpr decltype(auto) eval_impl(Id _) && {
                return _(eval)(detail::std::move(lf))(
                    _(eval)(detail::std::move(lx))
                );
            }
        };

        static constexpr detail::create<ap_result> ap_impl{};
    };

    //! Instance of `Functor` for `Lazy`.
    //!
    //! `fmap`ing a function to a lazy value returns the result of applying
    //! the function as a lazy value.
    //!
    //! ### Example
    //! @snippet example/lazy/functor.cpp fmap
    template <>
    struct Functor::instance<Lazy> : Functor::fmap_mcd {
        template <typename LX, typename F>
        static constexpr decltype(auto) fmap_impl(LX&& lx, F&& f) {
            return ap(lazy(detail::std::forward<F>(f)),
                      detail::std::forward<LX>(lx));
        }
    };

    //! Instance of `Monad` for `Lazy`.
    //!
    //! The `Lazy` monad allows combining lazy computations into larger
    //! lazy computations.
    //!
    //! ### Example
    //! @snippet example/lazy/monad.cpp main
    template <>
    struct Monad::instance<Lazy> : Monad::flatten_mcd<Lazy> {
        static constexpr auto flatten_impl = lazy(compose(eval, eval));
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_HPP
