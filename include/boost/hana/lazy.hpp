/*!
@file
Defines `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_HPP
#define BOOST_HANA_LAZY_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! The `Lazy` data type implements superficial laziness via a monadic
    //! interface.
    //!
    //! It is important to understand that the laziness implemented by `Lazy`
    //! is only superficial; only function applications made inside the `Lazy`
    //! monad can be made lazy, not all their subexpressions.
    //!
    //! ----------------------------------------------------------------------
    //!
    //! ## Instance of
    //! `Functor`, `Applicative` and `Monad`
    //!
    //! @note
    //! `Lazy` only instantiates a few type classes because providing more
    //! functionality would require evaluating the lazy values in most cases.
    //! Since this raises some issues such as side effects and memoization,
    //! the data type is kept simple.
    struct Lazy { };

    //! Evaluate a lazy value and return it.
    //! @relates Lazy
    BOOST_HANA_CONSTEXPR_LAMBDA auto eval = [](auto lx) {
        return lx.storage([](auto x) { return x; });
    };

    namespace lazy_detail {
        template <typename Storage, typename = operators::enable>
        struct lazy {
            Storage storage;
            using hana_datatype = Lazy;

            template <typename ...Xs>
            constexpr auto operator()(Xs ...xs) const {
                auto new_storage = [=](auto _) {
                    return _(eval)(*this)(xs...);
                };
                return lazy<decltype(new_storage)>{new_storage};
            }
        };
    }

    //! Lifts a normal value to a lazy one.
    //! @relates Lazy
    //!
    //! Additionally, `lazy(f)` is a function such that `lazy(f)(x1, ..., xN)`
    //! is equivalent to `ap(lazy(f), lift<Lazy>(x1), ..., lift<Lazy>(xN))`,
    //! which in turn is equivalent to `lazy(f(x1, ..., xN))`, except for the
    //! fact that the inner call to `f` is evaluated lazily. Note that
    //! `lazy(f)()` is equivalent to `lazy(f())`, with the inner call to
    //! `f` being evaluated lazily. This is provided for convenience even
    //! though `ap(lazy(f))` would be invalid because `ap` requires 2
    //! arguments or more.
    //!
    //! ### Example
    //! @snippet example/lazy/lazy.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy = [](auto x) {
        auto storage = [=](auto _) { return x; };
        return lazy_detail::lazy<decltype(storage)>{storage};
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
            auto storage = [=](auto _) { return f(_(eval)(lx)); };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }
    };

    //! @details
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

    //! @details
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
