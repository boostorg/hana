/*!
@file
Forward declares `boost::hana::Traversable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TRAVERSABLE_HPP
#define BOOST_HANA_FWD_TRAVERSABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/fwd/applicative.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Traversable` concept represents types admitting a structure-
    //! preserving right fold with an `Applicative`.
    //!
    //! Intuitively, the Traversable concept provides the minimal requirement
    //! for commuting two Applicatives, i.e. for turning a `F<G<T>>` into a
    //! `G<F<T>>`, where `F` and `G` are two Applicatives. For example, this
    //! could allow turning a tree of lists into a list of trees, because
    //! the tree is Traversable and the list is (in particular) an Applicative.
    //!
    //! The ability to commute Applicatives is fundamental when trying to
    //! compose Monads. For example, suppose that `M` and `N` are Monads (thus
    //! they are also Applicatives), and that we want to build the composition
    //! of `M` and `N` given by `Z<T> = M<N<T>>` for any type `T`. If we want
    //! the composition to be a Monad, we will need to implement the `flatten`
    //! method, whose pseudo type is
    //! @code
    //!     flatten : Z<Z<T>> -> Z<T>
    //! @endcode
    //! which is equivalent to
    //! @code
    //!     flatten : M<N<M<N<T>>>> -> M<N<T>>
    //! @endcode
    //! To be able to implement `flatten`, one has to be able to commute
    //! `N` and `M`, and then use the `flatten` of each Monad `M` and `N`
    //! to produce a value of the correct type. In other words, we want to do
    //! @code
    //!     M<N<M<N<T>>>>   ->   M<M<N<N<T>>>>  (commute M and N)
    //!                     ->   M<M<N<T>>>     (flatten N)
    //!                     ->   M<N<T>>        (flatten M)
    //! @endcode
    //!
    //! The ability to commute Applicatives in this way is exactly what the
    //! `sequence` method provides.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `sequence`\n
    //! When `sequence` is defined, `traverse` can be obtained by setting
    //! @code
    //!     traverse<A>(xs, f) = sequence<A>(transform(xs, f))
    //! @endcode
    //!
    //! 2. `traverse`\n
    //! When `traverse` is defined, `sequence` can be obtained by setting
    //! @code
    //!     sequence<A>(xs) = traverse<A>(xs, id)
    //! @endcode
    //!
    //!
    //! Superclass
    //! ----------
    //! `Functor`
    //!
    //!
    //! Laws
    //! ----
    //! @todo
    //! Write the laws for this concept. Can be based on [1]. The problem is
    //! that the current data type system is too weak to write complex laws,
    //! because we don't even officially have parameterized data types.
    //!
    //! [1]: https://www.haskell.org/pipermail/libraries/2012-October/018706.html
    struct Traversable { };

    //! Combine the applicatives in a structure from left to right and
    //! collect the results.
    //! @relates Traversable
    //!
    //!
    //! @tparam A
    //! The data type (an `Applicative`) of the elements in the structure.
    //! We must specify this data type explicitly because the current data
    //! type system is not powerful enough to let us peek into the contents
    //! of the traversable.
    //!
    //! @param xs
    //! The structure containing the `Applicative`s to combine.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/traversable.cpp sequence
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto sequence = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct sequence_impl;

    template <typename A>
    struct _sequence {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(models<Applicative, A>{},
        "hana::sequence<A>(xs) requires A to be an Applicative");
#endif
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(models<Traversable, typename datatype<Xs>::type>{},
            "hana::sequence<A>(xs) requires xs to be a Traversable");
#endif
            return sequence_impl<typename datatype<Xs>::type>::template apply<A>(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    template <typename A>
    constexpr _sequence<A> sequence{};
#endif

    //! Map each element of a structure to an `Applicative`, and then do
    //! the same as `sequence`.
    //! @relates Traversable
    //!
    //!
    //! @tparam A
    //! The data type (an `Applicative`) of an object returned by `f` when
    //! called with an element of the structure. We must specify this data
    //! type explicitly because the current data type system is not powerful
    //! enough to let us peek into the data type returned by `f`.
    //!
    //! @param xs
    //! The structure to be mapped over and then `sequence`d.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the structure
    //! and returning an `Applicative` that will then be combined as per
    //! `sequence`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/traversable.cpp traverse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto traverse = [](auto&& xs, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct traverse_impl;

    template <typename A>
    struct _traverse {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(models<Applicative, A>{},
        "hana::traverse<A>(xs, f) requires A to be an Applicative");
#endif
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(models<Traversable, typename datatype<Xs>::type>{},
            "hana::traverse<A>(xs, f) requires xs to be a Traversable");
#endif
            return traverse_impl<typename datatype<Xs>::type>::template apply<A>(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename A>
    constexpr _traverse<A> traverse{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TRAVERSABLE_HPP
