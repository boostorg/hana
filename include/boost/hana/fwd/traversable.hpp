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
#include <boost/hana/fwd/applicative.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    // Use long comment below to avoid multi-line comment warnings on GCC
    // because of Latex double backslash.

    /*!
    @ingroup group-concepts
    The `Traversable` concept represents types admitting a
    structure-preserving right fold with an `Applicative`.

    Intuitively, the Traversable concept provides the minimal requirement
    for commuting two Applicatives, i.e. for turning a `F(G(T))` into a
    `G(F(T))`, where `F` and `G` are two Applicatives. For example, this
    could allow turning a tree of lists into a list of trees, because
    the tree is Traversable and the list is (in particular) an Applicative.

    The ability to commute structures is fundamental when trying to
    compose Monads. For example, suppose that `M` and `N` are Monads (thus
    they are also Applicatives), and that we want to build the composition
    of `M` and `N` given by `Z(T) = M(N(T))` for any type `T`. If we want
    the composition to be a Monad, we will need to implement the `flatten`
    method, whose signature is then
    @f[
        \mathrm{flatten} : Z(Z(T)) \to Z(T)
    @f]

    which is equivalent to
    @f[
        \mathrm{flatten} : M(N(M(N(T)))) \to M(N(T))
    @f]

    To be able to implement `flatten`, one has to be able to commute
    `N` and `M`, and then use the `flatten` of each Monad `M` and `N`
    to produce a value of the correct type. In other words, we want to do
    @f{align*}{
        M(N(M(N(T)))) &\to M(M(N(N(T)))) & (\text{commute $M$ and $N$}) \\
                      &\to M(M(N(T)))    & (\text{flatten $N$})         \\
                      &\to M(N(T))       & (\text{flatten $M$})
    @f}

    Hence, the composition of two monads `M` and `N` will be a Monad
    whenever `M` is Traversable. The ability to commute structures in
    this way is exactly what the `sequence` method provides.


    Superclasses
    ------------
    `Functor`, `Foldable`


    Minimal complete definition
    ---------------------------
    1. `sequence`\n
    When `sequence` is defined, `traverse` can be obtained by setting
    @code
        traverse<A>(xs, f) = sequence<A>(transform(xs, f))
    @endcode

    2. `traverse`\n
    When `traverse` is defined, `sequence` can be obtained by setting
    @code
        sequence<A>(xs) = traverse<A>(xs, id)
    @endcode


    Concrete models
    ---------------
    `Either`, `Maybe`, `Tuple`


    Laws
    ----
    In the laws below, we use the `Identity` and `Compose` functors
    defined [here][1]. Note that those functors are not provided by
    Hana and they are only used here for documentation purposes.

    Let `A`, `B` be arbitrary Applicatives, and let `xs` be a Traversable
    of data type `T(A(X))`. Then, for any @ref applicative-transformation
    @f$ f : A(X) \to B(X) @f$, the following must hold. First, `sequence`
    must _play well_ with `transform`:
    @code
        // naturality
        f(sequence<A>(xs)) == sequence<B>(transform(xs, f))
    @endcode

    Second, using `sequence` with the Identity functor must not do
    anything special, which means that `sequence` can't make up
    arbitrary effects:
    @code
        // identity
        sequence<Identity>(transform(xs, make<Identity>)) == make<Identity>(xs)
    @endcode

    Finally, for any Traversable `xxs` of data type `T(A(B(X)))`, doing
    two traversals in sequence (commuting from `T(A(B(X)))` to `A(B(T(X)))`)
    must be collapsible into a single traversal:
    @code
        // composition
        sequence<Compose>(transform(xxs, make<Compose>))
            == make<Compose>(transform(sequence<A>(xxs), sequence<B>))
    @endcode

    [1]: https://hackage.haskell.org/package/base-4.7.0.2/docs/Data-Traversable.html
    */
    struct Traversable { };

    //! Combine the applicatives in a structure from left to right and
    //! collect the results.
    //! @relates Traversable
    //!
    //!
    //! Signature
    //! ---------
    //! Given an Applicative `A`, a Traversable `T` and an arbitrary data type
    //! `X`, the signature is @f$ \mathrm{sequence_A} : T(A(X)) \to A(T(X)) @f$.
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
        static_assert(_models<Applicative, A>{},
        "hana::sequence<A> requires A to be an Applicative");
    #endif

        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
            using T = typename datatype<Xs>::type;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Traversable, T>{},
            "hana::sequence<A>(xs) requires xs to be a Traversable");
        #endif

            return sequence_impl<T>::template apply<A>(static_cast<Xs&&>(xs));
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
    //! Signature
    //! ---------
    //! Given an Applicative `A`, a Traversable `T` and a
    //! function @f$ f : X \to A(Y) @f$, the signature is
    //! @f$ \mathrm{traverse_A} : T(X) \times (X \to A(Y)) \to A(T(Y)) @f$.
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
    template <typename T, typename = void>
    struct traverse_impl;

    template <typename A>
    struct _traverse {

    #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<Applicative, A>{},
        "hana::traverse<A> requires A to be an Applicative");
    #endif

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            using T = typename datatype<Xs>::type;
            using Traverse = traverse_impl<T>;

        #ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Traversable, T>{},
            "hana::traverse<A>(xs, f) requires xs to be a Traversable");
        #endif

            return Traverse::template apply<A>(static_cast<Xs&&>(xs),
                                               static_cast<F&&>(f));
        }
    };

    template <typename A>
    constexpr _traverse<A> traverse{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TRAVERSABLE_HPP
