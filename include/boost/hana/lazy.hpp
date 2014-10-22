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
        template <typename X>
        static constexpr auto lift_impl(X x) {
            auto storage = [=](auto) { return x; };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }

        template <typename LF, typename LX>
        static constexpr auto ap_impl(LF lf, LX lx) {
            auto storage = [=](auto _) { return _(eval)(lf)(_(eval)(lx)); };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }
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
        static constexpr auto fmap_impl(LX lx, F f) {
            auto storage = [=](auto _) { return f(_(eval)(lx)); };
            return lazy_detail::lazy<decltype(storage)>{storage};
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
        template <typename LLX>
        static constexpr auto flatten_impl(LLX llx) {
            auto storage = [=](auto _) {
                return eval(_(eval)(llx));
            };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }

        template <typename LX, typename F>
        static constexpr auto bind_impl(LX lx, F f) {
            auto storage = [=](auto _) {
                return eval(f(_(eval)(lx)));
            };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_HPP
