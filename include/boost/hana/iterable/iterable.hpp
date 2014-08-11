/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_ITERABLE_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


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
    //!     head(it) == head(to<List>(it))
    //!     to<List>(tail(it)) == tail(to<List>(it))
    //!     is_empty(it) if and only if is_empty(to<List>(it))
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
    //! @snippet example/iterable/head.cpp main
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
    //! @snippet example/iterable/tail.cpp main
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
    //! @snippet example/iterable/is_empty.cpp main
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
    //! @snippet example/iterable/at.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto at = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::at_impl(
            detail::std::forward<decltype(n)>(n),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };

    //! Return the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! ### Example
    //! @snippet example/iterable/last.cpp main
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
    //! @snippet example/iterable/drop.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop = [](auto&& n, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_impl(
            detail::std::forward<decltype(n)>(n),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };

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
    //! @snippet example/iterable/drop_while.cpp main
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
    //! @snippet example/iterable/drop_until.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_until = [](auto&& predicate, auto&& iterable) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_until_impl(
            detail::std::forward<decltype(predicate)>(predicate),
            detail::std::forward<decltype(iterable)>(iterable)
        );
    };

    //! Perform an action on each element of an iterable, discarding
    //! the result each time.
    //! @relates Iterable
    //!
    //! Iteration is done in the same order as per repeatedly applying
    //! `tail` to the iterable. If the iterable is not finite, this method
    //! will not terminate.
    //!
    //!
    //! @param iterable
    //! The structure on which iteration is done.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the iterable.
    //! The result of `f(x)`, whatever it is, is ignored.
    //!
    //!
    //! ### Example
    //! @snippet example/iterable/for_each.cpp main
    //!
    //! @todo
    //! The presence of implicit side effects in this function might be a
    //! smell that it should be moved to a different type class and handled
    //! through `Monad`s.
    BOOST_HANA_CONSTEXPR_LAMBDA auto for_each = [](auto&& iterable, auto&& f) -> decltype(auto) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::for_each_impl(
            detail::std::forward<decltype(iterable)>(iterable),
            detail::std::forward<decltype(f)>(f)
        );
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_ITERABLE_HPP
