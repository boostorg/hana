/*!
@file
Forward declares `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_LAZY_HPP
#define BOOST_HANA_FWD_LAZY_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/fwd/monad.hpp>


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
    struct Lazy { struct hana_enabled_operators : Monad { }; };

    //! Evaluate a lazy value and return it.
    //! @relates Lazy
    BOOST_HANA_CONSTEXPR_LAMBDA auto eval = [](auto lx) {
        return lx.storage(id);
    };

    namespace lazy_detail {
        template <typename Storage, typename = operators::enable_adl>
        struct lazy {
            Storage storage;
            struct hana { using datatype = Lazy; };

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
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_LAZY_HPP
