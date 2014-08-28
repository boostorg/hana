/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_ITERABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    namespace detail {
        struct FoldableFromIterable;
    }

    //! @ingroup group-typeclasses
    //! `Iterable` represents data structures supporting external iteration.
    //!
    //! ### Laws
    //! In a sense, `Foldable` represents data structures supporting internal
    //! iteration with the ability to accumulate a result. We request that
    //! instances of both `Iterable` and `Foldable` provide consistent
    //! instances. Hence, for any finite `Iterable` _and_ `Foldable` `it`,
    //! the following laws must be satisfied:
    //! @code
    //!     head(it) == head(to<Tuple>(it))
    //!     to<Tuple>(tail(it)) == tail(to<Tuple>(it))
    //!     is_empty(it) if and only if is_empty(to<Tuple>(it))
    //! @endcode
    //!
    //! This is basically saying that linearizing a `Foldable` and then
    //! iterating through it should be equivalent to iterating through it
    //! in the first place.
    struct Iterable {
        BOOST_HANA_TYPECLASS(Iterable);
        struct mcd;
    };

    //! Return the first element of a non-empty iterable.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp head
    BOOST_HANA_CONSTEXPR_LAMBDA auto head = [](auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::head_impl(detail::std::forward<decltype(iterable)>(iterable));
    };

    //! Return a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp tail
    BOOST_HANA_CONSTEXPR_LAMBDA auto tail = [](auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::tail_impl(detail::std::forward<decltype(iterable)>(iterable));
    };

    //! Return whether the iterable is empty.
    //! @relates Iterable
    //!
    //! Specificaly, `is_empty` must return a [compile-time]
    //! (@ref Logical_terminology) `Logical` representing
    //! whether the iterable is empty.
    //!
    //! ### Example
    //! @snippet example/iterable.cpp is_empty
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_empty = [](auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::is_empty_impl(detail::std::forward<decltype(iterable)>(iterable));
    };

    //! Return the `n`th element of an iterable.
    //! @relates Iterable
    //!
    //!
    //! @param n
    //! A non-negative `Integral` representing the 0-based index of the
    //! element to return. The iterable must contain at least `n + 1`
    //! elements.
    //!
    //! @param iterable
    //! The iterable in which an element is fetched.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp at
    BOOST_HANA_CONSTEXPR_LAMBDA auto at = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::at_impl(
            detail::std::forward<decltype(n)>(n),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };

    //! Equivalent to `at`; provided for convenience.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp at_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto at_c = [](auto&& iterable) -> decltype(auto) {
        return at(size_t<n>, std::forward<decltype(iterable)>(iterable));
    };
#else
    namespace iterable_detail {
        template <detail::std::size_t n>
        struct at_c {
            template <typename Xs>
            constexpr decltype(auto) operator()(Xs&& xs) const
            { return at(size_t<n>, detail::std::forward<Xs>(xs)); }
        };
    }

    template <detail::std::size_t n>
    constexpr iterable_detail::at_c<n> at_c{};
#endif

    //! Return the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp last
    BOOST_HANA_CONSTEXPR_LAMBDA auto last = [](auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::last_impl(detail::std::forward<decltype(iterable)>(iterable));
    };

    //! Drop the first `n` elements of an iterable and return the rest.
    //! @relates Iterable
    //!
    //!
    //! @param n
    //! A non-negative `Integral` representing the number of elements to be
    //! dropped from the iterable. If `n` is greater than the number of
    //! elements in the iterable, the returned iterable is empty.
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_impl(
            detail::std::forward<decltype(n)>(n),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };

    //! Equivalent to `drop`; provided for convenience.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop_c
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto drop_c = [](auto&& iterable) -> decltype(auto) {
        return drop(size_t<n>, std::forward<decltype(iterable)>(iterable));
    };
#else
    namespace iterable_detail {
        template <detail::std::size_t n>
        struct drop_c {
            template <typename Xs>
            constexpr decltype(auto) operator()(Xs&& xs) const
            { return drop(size_t<n>, detail::std::forward<Xs>(xs)); }
        };
    }

    template <detail::std::size_t n>
    constexpr iterable_detail::drop_c<n> drop_c{};
#endif

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is not satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //! If the iterable is not finite, the `predicate` has to return a false-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! be dropped from the structure. In the current version of the library,
    //! `predicate` should return a [compile-time](@ref Logical_terminology)
    //! `Logical`.
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop_while
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_while = [](auto&& predicate, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_while_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };

    //! Drop elements from an iterable up to, but excluding, the first
    //! element for which the `predicate` is satisfied.
    //! @relates Iterable
    //!
    //! Specifically, `drop_until` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is satisfied.
    //! If the iterable is not finite, the `predicate` has to return a true-
    //! valued `Logical` at a finite index for this method to return.
    //!
    //! @note
    //! This is effectively equivalent to `drop_while` with a negated
    //! `predicate`.
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` and
    //! subsequent elements should be kept in the structure. In the current
    //! version of the library, `predicate` should return a
    //! [compile-time](@ref Logical_terminology) `Logical`.
    //!
    //! @param iterable
    //! The iterable from which elements are dropped.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable.cpp drop_until
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_until = [](auto&& predicate, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_until_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_ITERABLE_HPP
