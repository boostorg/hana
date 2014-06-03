/*!
@file
Defines `boost::hana::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ITERABLE_HPP
#define BOOST_HANA_ITERABLE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>


namespace boost { namespace hana {
    /*!
    @typeclass{Iterable}
    @{
    Data structures allowing external iteration.

    @mcd
    `head`, `tail` and `is_empty`

    @note
    All indexing is 0-based.

    @todo
    - What about infinite `Iterable`s?
    - There are probably tons of laws that must be respected?
    - How to document/provide default instantiations for some type classes
    like `Foldable`?
    - Instead of having a lot of methods, maybe some of the functions below
    should just be implemented as functions using the mcd, as in the MPL11?
     */
    template <typename T>
    struct Iterable;

    //! Return the first element of a non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto head = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::head_impl(iterable);
    };

    //! Return a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto tail = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::tail_impl(iterable);
    };

    //! Return whether the iterable is empty.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_empty = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::is_empty_impl(iterable);
    };

    //! Return the `n`th element of an iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto at = [](auto n, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::at_impl(n, iterable);
    };

    //! Return the last element of a non-empty iterable.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto last = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::last_impl(iterable);
    };

    //! Return the number of elements in a finite iterable.
    //! @method{Iterable}
    //!
    //! @todo
    //! - Should this be moved into `Foldable`?
    //! - Implement this with `foldl`:
    //! @code
    //!     foldl([](auto n, auto) { return n + size_t<1>; }, size_t<0>, iterable);
    //! @endcode
    BOOST_HANA_CONSTEXPR_LAMBDA auto length = [](auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::length_impl(iterable);
    };

    //! Drop the first `n` elements of an iterable and return the rest.
    //!
    //! `n` must be a non-negative `Integral` representing the number of
    //! elements to be dropped from the iterable. If `n` is greater than
    //! the length of the iterable, the returned iterable is empty.
    //!
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop = [](auto n, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::drop_impl(n, iterable);
    };

    //! Drop elements from an iterable up to, but not including, the first
    //! element for which the `predicate` is not satisfied.
    //!
    //! Specifically, `drop_while` returns an iterable containing all the
    //! elements of the original iterable except for those in the range
    //! delimited by [`head`, `e`), where `head` is the first element and
    //! `e` is the first element for which the `predicate` is not satisfied.
    //!
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_while = [](auto predicate, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::drop_while_impl(predicate, iterable);
    };

    //! Equivalent to `drop_while` with a negated `predicate`.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_until = [](auto predicate, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::drop_until_impl(predicate, iterable);
    };

    //! Return `just` the first element satisfying the `predicate`, or `nothing`
    //! if there is no such element.
    //! @method{Iterable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto find = [](auto predicate, auto iterable) {
        return Iterable<datatype_t<decltype(iterable)>>::find_impl(predicate, iterable);
    };

    //! @}

    template <>
    struct defaults<Iterable> {
        template <typename Index, typename Iterable_>
        static constexpr auto at_impl(Index n, Iterable_ iterable) {
            return if_(n == size_t<0>,
                [](auto n, auto it) { return head(it); },
                [](auto n, auto it) { return at_impl(n - size_t<1>, tail(it)); }
            )(n, iterable);
        }

        template <typename Iterable_>
        static constexpr auto last_impl(Iterable_ iterable) {
            return if_(is_empty(tail(iterable)),
                head,
                [](auto it) { return last_impl(tail(it)); }
            )(iterable);
        }

        template <typename Iterable_>
        static constexpr auto length_impl(Iterable_ iterable) {
            return if_(is_empty(iterable),
                always(size_t<0>),
                [](auto it) { return size_t<1> + length_impl(tail(it)); }
            )(iterable);
        }

        template <typename N, typename Iterable_>
        static constexpr auto drop_impl(N n, Iterable_ iterable) {
            return if_(n == size_t<0> || is_empty(iterable),
                always(iterable),
                [](auto n, auto it) { return drop_impl(n - size_t<1>, tail(it)); }
            )(n, iterable);
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto drop_while_impl(Pred pred, Iterable_ iterable) {
            return if_(is_empty(iterable),
                always(iterable),
                [=](auto it) {
                    return if_(pred(head(it)),
                        [=](auto it) { return drop_while_impl(pred, tail(it)); },
                        always(it)
                    )(it);
                }
            )(iterable);
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto drop_until_impl(Pred pred, Iterable_ iterable) {
            return drop_while([=](auto x) { return !pred(x); }, iterable);
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto find_impl(Pred pred, Iterable_ iterable) {
            auto e = drop_until(pred, iterable);
            return if_(is_empty(e),
                always(nothing),
                compose(just, head)
            )(e);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
