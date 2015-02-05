/*!
@file
Forward declares `boost::hana::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TRAVERSABLE_HPP
#define BOOST_HANA_FWD_TRAVERSABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Types admitting a structure-preserving right fold with an `Applicative`.
    //!
    //! ### Requires
    //! `Functor`
    //!
    //! ### Laws
    //! Instances of `Traversable` must satisfy the following laws. For any
    //! [applicative transformation](@ref Applicative_terminology) `t`,
    //!
    //! @code
    //!     t(sequence(x)) == sequence(fmap(x, t))                              // naturality
    //!     sequence(fmap(x, Identity)) == Identity(x)                          // identity
    //!     sequence(fmap(x, Compose)) == Compose(fmap(sequence(x), sequence))  // composition
    //!     traverse(x, f) == sequence(fmap(x, f))
    //! @endcode
    //!
    //! where [Identity][] and [Compose][] are the identity functor and the
    //! composition of functors, respectively. Note that those two functors
    //! are not provided with the library right now.
    //!
    //! [Compose]: http://hackage.haskell.org/package/transformers-0.4.1.0/docs/Data-Functor-Compose.html
    //! [Identity]: http://hackage.haskell.org/package/transformers-0.4.1.0/docs/Data-Functor-Identity.html
    struct Traversable {
        BOOST_HANA_TYPECLASS(Traversable);
        struct traverse_mcd;
        template <typename T>
        struct list_mcd;
    };

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
    //! @param traversable
    //! The structure containing the `Applicative`s to combine.
    //!
    //!
    //! ### Example
    //! @snippet example/traversable.cpp sequence
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto sequence = [](auto&& traversable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct sequence_impl : sequence_impl<T, when<true>> { };

    template <typename Trav, bool condition>
    struct sequence_impl<Trav, when<condition>> {
        template <typename A, typename Xs>
        static constexpr decltype(auto) apply(Xs&& traversable) {
            return Traversable::instance<Trav>::template sequence_impl<A>(
                detail::std::forward<Xs>(traversable)
            );
        }
    };

    template <typename A>
    struct _sequence {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& traversable) const {
            return sequence_impl<datatype_t<Xs>>::template apply<A>(
                detail::std::forward<Xs>(traversable)
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
    //! @param traversable
    //! The structure to be mapped over and then `sequence`d.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the structure
    //! and returning an `Applicative` that will then be combined as per
    //! `sequence`.
    //!
    //!
    //! ### Example
    //! @snippet example/traversable.cpp traverse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto traverse = [](auto&& traversable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct traverse_impl : traverse_impl<T, when<true>> { };

    template <typename Trav, bool condition>
    struct traverse_impl<Trav, when<condition>> {
        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& traversable, F&& f) {
            return Traversable::instance<Trav>::template traverse_impl<A>(
                detail::std::forward<Xs>(traversable),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename A>
    struct _traverse {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& traversable, F&& f) const {
            return traverse_impl<datatype_t<Xs>>::template apply<A>(
                detail::std::forward<Xs>(traversable),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename A>
    constexpr _traverse<A> traverse{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TRAVERSABLE_HPP
