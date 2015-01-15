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
#include <boost/hana/core/method.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! Types admitting a structure-preserving right fold with an `Applicative`.
    //!
    //!
    //! Superclass
    //! ----------
    //! `Functor`
    //!
    //!
    //! Laws
    //! ----
    //! Instances of `Traversable` must satisfy the following laws. For any
    //! [applicative transformation](@ref Applicative_terminology) `t`,
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
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `traverse`
    //!
    //!
    //! [Compose]: http://hackage.haskell.org/package/transformers-0.4.1.0/docs/Data-Functor-Compose.html
    //! [Identity]: http://hackage.haskell.org/package/transformers-0.4.1.0/docs/Data-Functor-Identity.html
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
    BOOST_HANA_METHOD(sequence_impl);

    template <typename A>
    struct _sequence {
        template <typename T>
        constexpr decltype(auto) operator()(T&& traversable) const {
            return dispatch<sequence_impl<
                typename datatype<T>::type
            >>::template apply<A>(
                detail::std::forward<T>(traversable)
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
    BOOST_HANA_METHOD(traverse_impl);

    template <typename A>
    struct _traverse {
        template <typename T, typename F>
        constexpr decltype(auto) operator()(T&& traversable, F&& f) const {
            return dispatch<traverse_impl<
                typename datatype<T>::type
            >>::template apply<A>(
                detail::std::forward<T>(traversable),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename A>
    constexpr _traverse<A> traverse{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TRAVERSABLE_HPP
