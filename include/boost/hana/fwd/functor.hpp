/*!
@file
Forward declares `boost::hana::Functor`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FUNCTOR_HPP
#define BOOST_HANA_FWD_FUNCTOR_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Functor` concept represents types that can be mapped over.
    //!
    //! Intuitively, a [Functor][1] is some kind of box that can hold generic
    //! data and map a function over this data to create a new, transformed
    //! box. Because we are only interested in mapping a function over the
    //! contents of a black box, the only real requirement for being a functor
    //! is to provide a function which can do the mapping, along with a couple
    //! of guarantees that the mapping is well-behaved. Those requirements are
    //! made precise in the laws below. The pattern captured by `Functor` is
    //! very general, which makes it widely useful. A lot of objects can be
    //! made `Functor`s in one way or another, the most obvious example being
    //! sequences with the usual mapping of the function on each element.
    //!
    //! Functors are parametric data types which are parameterized over the
    //! data type of the objects they contain. Like everywhere else in Hana,
    //! this parametricity is only at the documentation level and it is not
    //! enforced.
    //!
    //! In this library, the mapping function is called `transform` after the
    //! `std::transform` algorithm, but other programming languages have given
    //! it different names (usually `map`).
    //!
    //! @note
    //! The word `Functor` comes from functional programming, where the
    //! concept has been used for a while, notably in the Haskell programming
    //! language. Haskell people borrowed the term from [category theory][2],
    //! which, broadly speaking, is a field of mathematics dealing with
    //! abstract structures and transformations between those structures.
    //!
    //!
    //! Laws
    //! ----
    //! Let `xs` be a Functor of data type `F(A)`,
    //!     \f$ f : A \to B \f$ and
    //!     \f$ g : B \to C \f$.
    //! The following laws must be satisfied:
    //! @code
    //!     transform(xs, id) == xs
    //!     transform(xs, compose(g, f)) == transform(transform(xs, f), g)
    //! @endcode
    //! The first line says that mapping the identity function should not do
    //! anything, which precludes the functor from doing something nasty
    //! behind the scenes. The second line states that mapping the composition
    //! of two functions is the same as mapping the first function, and then
    //! the second on the result. While the usual functor laws are usually
    //! restricted to the above, this library includes other convenience
    //! methods and they should satisfy the following equations.
    //! Let `xs` be a Functor of data type `F(A)`,
    //!     \f$ f : A \to A \f$,
    //!     \f$ \mathrm{pred} : A \to \mathrm{Bool} \f$
    //! for some `Logical` `Bool`, and `value` an object of data type `A`.
    //! Then,
    //! @code
    //!     adjust(xs, value, f) == adjust_if(xs, equal.to(value), f)
    //!     adjust_if(xs, pred, f) == transform(xs, [](x){
    //!         if pred(x) then f(x) else x
    //!     })
    //!     replace(xs, pred, value) == adjust_if(xs, pred, always(value))
    //!     fill(xs, value)          == replace(xs, always(true), value)
    //! @endcode
    //! The default definition of the methods will satisfy these equations.
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! 1. `transform`\n
    //! When `transform` is specified, `adjust_if` is defined analogously to
    //! @code
    //!     adjust_if(xs, pred, f) = transform(xs, [](x){
    //!         if pred(x) then f(x) else x
    //!     })
    //! @endcode
    //!
    //! 2. `adjust_if`\n
    //! When `adjust_if` is specified, `transform` is defined analogously to
    //! @code
    //!     transform(xs, f) = adjust_if(xs, always(true), f)
    //! @endcode
    //!
    //!
    //! Structure-preserving functions for Functors
    //! -------------------------------------------
    //! A mapping between two functors which also preserves the functor
    //! laws is called a natural transformation (the term comes from
    //! category theory).
    //!
    //! @todo
    //! Grok and then document natural transformations.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Functor
    //! [2]: http://en.wikipedia.org/wiki/Category_theory
    struct Functor { };

    //! %Map a function over a `Functor`.
    //! @relates Functor
    //!
    //!
    //! Signature
    //! ---------
    //! Given `F` a Functor, the signature is
    //! \f$
    //!     \mathrm{transform} : F(T) \times (T \to U) \to F(U)
    //! \f$
    //!
    //! @param xs
    //! The structure to map `f` over.
    //!
    //! @param f
    //! A function called as `f(x)` on element(s) `x` of the structure,
    //! and returning a new value to replace `x` in the structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functor.cpp transform
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/functor/transform.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto transform = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct transform_impl;

    struct _transform {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Functor, typename datatype<Xs>::type>{},
            "hana::transform(xs, f) requires xs to be a Functor");
#endif

            return transform_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _transform transform{};
#endif

    //! Apply a function on all the elements of a structure satisfying a
    //! `predicate`.
    //! @relates Functor
    //!
    //!
    //! Signature
    //! ---------
    //! Given `F` a Functor and `Bool` a Logical, the signature is
    //! \f$
    //!     \mathrm{adjust_if} : F(T) \times (T \to Bool) \times (T \to T) \to F(T)
    //! \f$
    //!
    //! @param xs
    //! The structure to adjust with `f`.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for element(s) `x` of the
    //! structure and returning a `Logical` representing whether `f`
    //! should be applied to `x`.
    //!
    //! @param f
    //! A function called as `f(x)` on element(s) of the structure for which
    //! the `predicate` returns a true-valued `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functor.cpp adjust_if
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/functor/adjust_if.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto adjust_if = [](auto&& xs, auto&& predicate, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct adjust_if_impl;

    struct _adjust_if {
        template <typename Xs, typename Pred, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Functor, typename datatype<Xs>::type>{},
            "hana::adjust_if(xs, pred, f) requires xs to be a Functor");
#endif
            return adjust_if_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _adjust_if adjust_if{};
#endif

    //! Apply a function on all the elements of a structure that compare
    //! equal to some value.
    //! @relates Functor
    //!
    //!
    //! Signature
    //! ---------
    //! Given `F` a Functor and `U` a type that can be compared with `T`'s,
    //! the signature is
    //! \f$
    //!     \mathrm{adjust} : F(T) \times U \times (T \to T) \to F(T)
    //! \f$
    //!
    //! @param xs
    //! The structure to adjust with `f`.
    //!
    //! @param value
    //! An object that is compared with each element `x` of the structure.
    //! Elements of the structure that compare equal to `value` are adjusted
    //! with the `f` function.
    //!
    //! @param f
    //! A function called as `f(x)` on the element(s) of the structure that
    //! compare equal to `value`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functor.cpp adjust
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/functor/adjust.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto adjust = [](auto&& xs, auto&& value, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct adjust_impl;

    struct _adjust {
        template <typename Xs, typename Value, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Functor, typename datatype<Xs>::type>{},
            "hana::adjust(xs, value, f) requires xs to be a Functor");
#endif
            return adjust_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Value>(value),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _adjust adjust{};
#endif

    //! Replace all the elements of a structure satisfying a `predicate`
    //! with a fixed value.
    //! @relates Functor
    //!
    //!
    //! Signature
    //! ---------
    //! Given `F` a Functor and `Bool` a Logical, the signature is
    //! \f$
    //!     \mathrm{replace} : F(T) \times (T \to Bool) \times T \to F(T)
    //! \f$
    //!
    //! @param xs
    //! The structure to replace elements of.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for element(s) `x` of the
    //! structure and returning a `Logical` representing whether `x`
    //! should be replaced by `value`.
    //!
    //! @param value
    //! A value by which every element `x` of the structure for which
    //! `predicate` returns a true-valued `Logical` is replaced.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functor.cpp replace
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/functor/replace.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto replace = [](auto&& xs, auto&& predicate, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct replace_impl;

    struct _replace {
        template <typename Xs, typename Pred, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred, Value&& value) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Functor, typename datatype<Xs>::type>{},
            "hana::replace(xs, pred, value) requires xs to be a Functor");
#endif
            return replace_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::forward<Value>(value)
            );
        }
    };

    constexpr _replace replace{};
#endif

    //! Replace all the elements of a structure with a fixed value.
    //! @relates Functor
    //!
    //!
    //! Signature
    //! ---------
    //! Given `F` a Functor, the signature is
    //! \f$
    //!     \mathrm{fill} : F(T) \times U \to F(U)
    //! \f$
    //!
    //! @param xs
    //! The structure to fill with a `value`.
    //!
    //! @param value
    //! A value by which every element `x` of the structure is replaced,
    //! unconditionally.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functor.cpp fill
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/functor/fill.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fill = [](auto&& xs, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct fill_impl;

    struct _fill {
        template <typename Xs, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Functor, typename datatype<Xs>::type>{},
            "hana::fill(xs, value) requires xs to be a Functor");
#endif
            return fill_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Value>(value)
            );
        }
    };

    constexpr _fill fill{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FUNCTOR_HPP
