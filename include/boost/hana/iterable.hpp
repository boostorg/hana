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
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Iterable;

    constexpr struct _head {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::head_impl(iterable); }
    } head{};

    constexpr struct _tail {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::tail_impl(iterable); }
    } tail{};

    constexpr struct _is_empty {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::is_empty_impl(iterable); }
    } is_empty{};

    constexpr struct _at {
        template <typename Index, typename Iterable_>
        constexpr auto operator()(Index n, Iterable_ iterable) const
        { return Iterable<Iterable_>::at_impl(n, iterable); }
    } at{};

    constexpr struct _last {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::last_impl(iterable); }
    } last{};

    constexpr struct _length {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::length_impl(iterable); }
    } length{};

    constexpr struct _drop {
        template <typename N, typename Iterable_>
        constexpr auto operator()(N n, Iterable_ iterable) const
        { return Iterable<Iterable_>::drop_impl(n, iterable); }
    } drop{};

    constexpr struct _any {
        template <typename Pred, typename Iterable_>
        constexpr auto operator()(Pred pred, Iterable_ iterable) const
        { return Iterable<Iterable_>::any_impl(pred, iterable); }
    } any{};

    constexpr struct _any_of {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::any_of_impl(iterable); }
    } any_of{};

    constexpr struct _all {
        template <typename Pred, typename Iterable_>
        constexpr auto operator()(Pred pred, Iterable_ iterable) const
        { return Iterable<Iterable_>::all_impl(pred, iterable); }
    } all{};

    constexpr struct _all_of {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::all_of_impl(iterable); }
    } all_of{};

    constexpr struct _none {
        template <typename Pred, typename Iterable_>
        constexpr auto operator()(Pred pred, Iterable_ iterable) const
        { return Iterable<Iterable_>::none_impl(pred, iterable); }
    } none{};

    constexpr struct _none_of {
        template <typename Iterable_>
        constexpr auto operator()(Iterable_ iterable) const
        { return Iterable<Iterable_>::none_of_impl(iterable); }
    } none_of{};


    template <>
    struct defaults<Iterable> {
        template <typename Index, typename Iterable_>
        static constexpr auto at_impl(Index n, Iterable_ iterable) {
            return if_(n == size_t<0>,
                [](auto n, auto it) { return head(it); },
                [](auto n, auto it) { return at(n - size_t<1>, tail(it)); }
            )(n, iterable);
        }

        template <typename Iterable_>
        static constexpr auto last_impl(Iterable_ iterable) {
            return if_(is_empty(tail(iterable)),
                head,
                compose(last, tail)
            )(iterable);
        }

        template <typename Iterable_>
        static constexpr auto length_impl(Iterable_ iterable) {
            return if_(is_empty(iterable),
                always(size_t<0>),
                [](auto it) { return size_t<1> + length(tail(it)); }
            )(iterable);
        }

        template <typename N, typename Iterable_>
        static constexpr auto drop_impl(N n, Iterable_ iterable) {
            return if_(n == size_t<0> || is_empty(iterable),
                always(iterable),
                [](auto n, auto it) { return drop(n - size_t<1>, tail(it)); }
            )(n, iterable);
        }

        // any, all, none
        template <typename Pred, typename Iterable_>
        static constexpr auto any_impl(Pred pred, Iterable_ iterable) {
            return if_(is_empty(iterable),
                always(false_),
                [](auto pred, auto it) {
                    return pred(head(it)) || any(pred, tail(it));
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
