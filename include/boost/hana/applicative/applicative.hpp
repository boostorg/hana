/*!
@file
Forward declares `boost::hana::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_APPLICATIVE_HPP
#define BOOST_HANA_APPLICATIVE_APPLICATIVE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functor/functor.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! `Applicative`s are `Functor`s with the ability to lift values and
    //! combine computations.
    //!
    //! ### Requires
    //! `Functor`
    //!
    //! @anchor Applicative_terminology
    //! ### Terminology
    //! An _applicative transformation_ `t` is a function from an `Applicative`
    //! `F` to an `Applicative` `G` preserving the `Applicative` operations:
    //!
    //! @code
    //!     t(lift<F>(x)) == lift<G>(x)
    //!     t(ap(x, y)) == ap(t(x), t(y))
    //! @endcode
    //!
    //! This term is defined here but used elsewhere (at least in the laws
    //! of `Traversable`).
    //!
    //! ### Laws
    //! For any `Applicative` `A`, the following laws must be satisfied:
    //!
    //! @code
    //!     ap(lift<A>(id), u) == u                          // identity
    //!     ap(lift<A>(compose), u, v, w) == ap(u, ap(v, w)) // composition
    //!     ap(lift<A>(f), lift<A>(x)) == lift<A>(f(x))      // homomorphism
    //!     ap(u, lift<A>(y)) == ap(lift<A>(apply(y)), u)    // interchange
    //! @endcode
    //!
    //! As a consequence of these laws, the `Functor` instance for `A` will
    //! satisfy
    //!
    //! @code
    //!     fmap(x, f) == ap(lift<A>(f), x)
    //! @endcode
    struct Applicative {
        BOOST_HANA_TYPECLASS(Applicative);
        struct mcd;
        template <typename T>
        struct list_mcd;
    };

    namespace applicative_detail {
        template <typename A>
        struct lift {
            template <typename X>
            constexpr decltype(auto) operator()(X&& x) const {
                return Applicative::instance<
                    A
                >::lift_impl(detail::std::forward<X>(x));
            }
        };

        struct ap {
            template <typename F, typename X>
            constexpr decltype(auto) operator()(F&& f, X&& x) const {
                return Applicative::instance<
                    datatype_t<F>
                >::ap_impl(detail::std::forward<F>(f), detail::std::forward<X>(x));
            }

            template <typename F, typename ...Xs>
            constexpr decltype(auto) operator()(F&& f, Xs&& ...xs) const {
                static_assert(sizeof...(xs) >= 1,
                "boost::hana::ap must be called with two arguments or more");
                return detail::variadic::foldl(
                    *this,
                    fmap(detail::std::forward<F>(f), curry<sizeof...(xs)>),
                    detail::std::forward<Xs>(xs)...
                );
            }
        };
    }

    //! Lifted application.
    //! @relates Applicative
    //!
    //! `ap` can be called with two arguments or more. Specifically,
    //! `ap(f, x1, ..., xN)` is equivalent to
    //! @code
    //!     foldl(list(x1, ..., xN), fmap(f, curry<N>), ap);
    //! @endcode
    //! where `ap(f, x)` (called with two arguments only) dispatches to the
    //! implementation in the type class. This basically means that applying
    //! a `N`-ary function with `ap` is equivalent to applying a curried
    //! binary function to each argument starting from the left with the
    //! `ap` provided in the type class.
    //!
    //!
    //! @param f
    //! An applicative containing function(s).
    //!
    //! @param x...
    //! Applicative(s) on which `f` is applied.
    //!
    //!
    //! ### Example
    //! @snippet example/applicative.cpp ap
    //!
    //! @todo
    //! Consider giving access to all the arguments to the type class
    //! implementation for performance purposes.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto ap = [](auto&& f, auto&& ...x) -> decltype(auto) {
        unspecified
    };
#else
    constexpr applicative_detail::ap ap{};
#endif

    //! Lift a value into the functor.
    //! @relates Applicative
    //!
    //!
    //! @param x
    //! The value to lift into the applicative.
    //!
    //! @tparam A
    //! The data type (an `Applicative`) to which the value is lifted.
    //!
    //!
    //! ### Example
    //! @snippet example/applicative.cpp lift
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto lift = [](auto&& x) -> decltype(auto) {
        unspecified
    };
#else
    template <typename A>
    constexpr applicative_detail::lift<A> lift{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_APPLICATIVE_HPP
