/*!
@file
Forward declares `boost::hana::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_TRAVERSABLE_HPP
#define BOOST_HANA_TRAVERSABLE_TRAVERSABLE_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


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
    //!     t(sequence(x)) == sequence(fmap(t, x))                              // naturality
    //!     sequence(fmap(Identity, x)) == Identity(x)                          // identity
    //!     sequence(fmap(Compose, x)) == Compose(fmap(sequence, sequence(x)))  // composition
    //!     traverse(f, x) == sequence(fmap(f, x))
    //! @endcode
    //!
    //! where `Identity` and [Compose][] are the identity functor and the
    //! composition of functors, respectively. Note that Compose is not
    //! provided with the library right now.
    //!
    //! @todo
    //! Define Compose in the library.
    //!
    //! [Compose]: http://hackage.haskell.org/package/transformers-0.4.1.0/docs/Data-Functor-Compose.html
    struct Traversable {
        BOOST_HANA_TYPECLASS(Traversable);
        struct traverse_mcd;
    };

    namespace traversable_detail {
        template <typename A>
        struct sequence {
            template <typename T>
            constexpr auto operator()(T traversable) const {
                return Traversable::instance<
                    datatype_t<decltype(traversable)>
                >::template sequence_impl<A>(traversable);
            }
        };

        template <typename A>
        struct traverse {
            template <typename F, typename T>
            constexpr auto operator()(F f, T traversable) const {
                return Traversable::instance<
                    datatype_t<decltype(traversable)>
                >::template traverse_impl<A>(f, traversable);
            }
        };
    }

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
    //! @snippet example/traversable/sequence.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto sequence = [](auto traversable) {
        return Traversable::instance<
            datatype_t<decltype(traversable)>
        >::template sequence_impl<A>(traversable);
    };
#else
    template <typename A>
    constexpr traversable_detail::sequence<A> sequence{};
#endif

    //! Map each element of a structure to an `Applicative`, and then do
    //! the same as `sequence`.
    //! @relates Traversable
    //!
    //! @tparam A
    //! The data type (an `Applicative`) of an object returned by `f` when
    //! called with an element of the structure. We must specify this data
    //! type explicitly because the current data type system is not powerful
    //! enough to let us peek into the data type returned by `f`.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the structure
    //! and returning an `Applicative` that will then be combined as per
    //! `sequence`.
    //!
    //! @param traversable
    //! The structure to be mapped over than then `sequence`d.
    //!
    //!
    //! ### Example
    //! @snippet example/traversable/traverse.cpp list
    //!
    //! ### Example
    //! @snippet example/traversable/traverse.cpp maybe
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename A>
    constexpr auto traverse = [](auto f, auto traversable) {
        return Traversable::instance<
            datatype_t<decltype(traversable)>
        >::template traverse_impl<A>(f, traversable);
    };
#else
    template <typename A>
    constexpr traversable_detail::traverse<A> traverse{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_TRAVERSABLE_HPP
