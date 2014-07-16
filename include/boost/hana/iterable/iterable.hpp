/*!
@file
Forward declares `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_ITERABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Data structures allowing external iteration.
    //!
    //! @note
    //! In the description of the methods, all indexing is 0-based.
    //!
    //! @todo
    //! - What about infinite `Iterable`s?
    //! - There are probably tons of laws that must be respected?
    struct Iterable {
        BOOST_HANA_TYPECLASS(Iterable);
        struct mcd;
        struct FoldableInstance;
    };

    //! Return the first element of a non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto head = [](auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::head_impl(iterable);
    };

    //! Return a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto tail = [](auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::tail_impl(iterable);
    };

    //! Return whether the iterable is empty.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_empty = [](auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::is_empty_impl(iterable);
    };

    //! Return the `n`th element of an iterable.
    //! @method{Iterable}
    //!
    //! ### Example
    //! @snippet example/list/iterable/at.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto at = [](auto n, auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::at_impl(n, iterable);
    };

    //! Return the last element of a non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto last = [](auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::last_impl(iterable);
    };

    //! Drop the first `n` elements of an iterable and return the rest.
    //! @method{Iterable}
    //!
    //! `n` must be a non-negative `Integral` representing the number of
    //! elements to be dropped from the iterable. If `n` is greater than
    //! the length of the iterable, the returned iterable is empty.
    //!
    //! ### Example
    //! @snippet example/list/iterable/drop.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop = [](auto n, auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_impl(n, iterable);
    };

    //! Drop elements from an iterable up to, but not including, the first
    //! element for which the `predicate` is not satisfied.
    //! @method{Iterable}
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //!
    //! ### Example
    //! @snippet example/range/iterable/drop_while.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_while = [](auto predicate, auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_while_impl(predicate, iterable);
    };

    //! Equivalent to `drop_while` with a negated `predicate`.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_until = [](auto predicate, auto iterable) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::drop_until_impl(predicate, iterable);
    };

    //! Perform an action on each element of the iterable, discarding the
    //! result each time.
    //! @method{Iterable}
    //!
    //! ### Example
    //! @snippet example/list/iterable/for_each.cpp main
    //!
    //! @todo This should probably be in a future `Traversable` type class.
    BOOST_HANA_CONSTEXPR_LAMBDA auto for_each = [](auto iterable, auto f) {
        return Iterable::instance<
            datatype_t<decltype(iterable)>
        >::for_each_impl(iterable, f);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_ITERABLE_HPP
