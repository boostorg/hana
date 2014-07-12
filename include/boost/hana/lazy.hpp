/*!
@file
Defines `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_HPP
#define BOOST_HANA_LAZY_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! The `Lazy` data type implements laziness via a monadic interface.
    //!
    //! ----------------------------------------------------------------------
    //!
    //! ## Instance of
    //! `Functor`, `Applicative` and `Monad`.
    struct Lazy { };

    //! Create a function returning a lazy value.
    //! @relates Lazy
    //!
    //! Specifically, `lazy(f)(x)` is equivalent to
    //! `ap(lift<Lazy>(f), lift<Lazy>(x))`.
    //!
    //! ### Example
    //! @snippet example/lazy/lazy.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy = [](auto f) {
        return [=](auto ...x) {
            return detail::wrap<Lazy>([=](auto _) { return _(f)(x...); });
        };
    };

    //! Evaluate a lazy value and return it.
    //! @relates Lazy
    BOOST_HANA_CONSTEXPR_LAMBDA auto eval = [](auto lx) {
        return detail::unwrap(lx)([](auto x) { return x; });
    };

    //! @details
    //! `fmap`ing a function to a lazy value returns the result of applying
    //! the function as a lazy value.
    //!
    //! ### Example
    //! @snippet example/lazy/functor/fmap.cpp main
    template <>
    struct Functor::instance<Lazy> : Functor::fmap_mcd {
        template <typename F, typename LX>
        static constexpr auto fmap_impl(F f, LX lx) {
            return detail::wrap<Lazy>([=](auto _) {
                return f(_(eval)(lx));
            });
        }
    };

    //! @details
    //! A normal value can be lifted into a lazy value by using `lift<Lazy>`.
    //! A lazy function can be lazily applied to a lazy value by using `ap`.
    template <>
    struct Applicative::instance<Lazy> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x) {
            return detail::wrap<Lazy>([=](auto) { return x; });
        }

        template <typename LF, typename LX>
        static constexpr auto ap_impl(LF lf, LX lx) {
            return detail::wrap<Lazy>([=](auto _) {
                return _(eval)(lf)(_(eval)(lx));
            });
        }
    };

    //! @details
    //! A lazy-lazy value can be made into a lazy value with `flatten`.
    template <>
    struct Monad::instance<Lazy> : Monad::flatten_mcd {
        template <typename LLX>
        static constexpr auto flatten_impl(LLX llx) {
            return eval(llx);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_HPP
