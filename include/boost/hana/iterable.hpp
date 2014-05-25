/*!
 * @file
 * Defines `boost::hana::Iterable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
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
    //! @ingroup typeclasses
    template <typename T>
    struct Iterable;


    BOOST_HANA_CONSTEXPR_LAMBDA auto head = [](auto iterable) {
        return Iterable<decltype(iterable)>::head_impl(iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto tail = [](auto iterable) {
        return Iterable<decltype(iterable)>::tail_impl(iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto is_empty = [](auto iterable) {
        return Iterable<decltype(iterable)>::is_empty_impl(iterable);
    };


    BOOST_HANA_CONSTEXPR_LAMBDA auto at = [](auto n, auto iterable) {
        return Iterable<decltype(iterable)>::at_impl(n, iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto last = [](auto iterable) {
        return Iterable<decltype(iterable)>::last_impl(iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto length = [](auto iterable) {
        return Iterable<decltype(iterable)>::length_impl(iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto drop = [](auto n, auto iterable) {
        return Iterable<decltype(iterable)>::drop_impl(n, iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto drop_while = [](auto pred, auto iterable) {
        return Iterable<decltype(iterable)>::drop_while_impl(pred, iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto any = [](auto pred, auto iterable) {
        return Iterable<decltype(iterable)>::any_impl(pred, iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto any_of = [](auto iterable) {
        return Iterable<decltype(iterable)>::any_of_impl(iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto all = [](auto pred, auto iterable) {
        return Iterable<decltype(iterable)>::all_impl(pred, iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto all_of = [](auto iterable) {
        return Iterable<decltype(iterable)>::all_of_impl(iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto none = [](auto pred, auto iterable) {
        return Iterable<decltype(iterable)>::none_impl(pred, iterable);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto none_of = [](auto iterable) {
        return Iterable<decltype(iterable)>::none_of_impl(iterable);
    };


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

        // any, all, none
        template <typename Pred, typename Iterable_>
        static constexpr auto any_impl(Pred pred, Iterable_ iterable) {
            return if_(is_empty(iterable),
                always(false_),
                [](auto pred, auto it) {
                    return pred(head(it)) || any_impl(pred, tail(it));
                }
            )(pred, iterable);
        }

        template <typename Pred, typename Iterable_>
        static constexpr auto all_impl(Pred pred, Iterable_ iterable)
        { return !any([=](auto x) { return !pred(x); }, iterable); }

        template <typename Pred, typename Iterable_>
        static constexpr auto none_impl(Pred pred, Iterable_ iterable)
        { return !any(pred, iterable); }


        // any_of, all_of, none_of
        template <typename Iterable_>
        static constexpr auto any_of_impl(Iterable_ iterable)
        { return any(id, iterable); }

        template <typename Iterable_>
        static constexpr auto all_of_impl(Iterable_ iterable)
        { return all(id, iterable); }

        template <typename Iterable_>
        static constexpr auto none_of_impl(Iterable_ iterable)
        { return none(id, iterable); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
