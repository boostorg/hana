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
#include <boost/hana/integral.hpp>


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
        static constexpr auto at_impl(Index n, Iterable_ iterable)
        { return at(n - size_t<1>, tail(iterable)); }

        template <typename T, typename Iterable_>
        static constexpr auto at_impl(Integral<T, 0>, Iterable_ iterable)
        { return head(iterable); }


        template <typename Iterable_>
        static constexpr auto last_helper(Iterable_ iterable, Bool<true>)
        { return head(iterable); }

        template <typename Iterable_>
        static constexpr auto last_helper(Iterable_ iterable, Bool<false>)
        { return last(tail(iterable)); }

        template <typename Iterable_>
        static constexpr auto last_impl(Iterable_ iterable)
        { return last_helper(iterable, is_empty(tail(iterable))); }


        template <typename Iterable_>
        static constexpr auto length_helper(Iterable_ iterable, Bool<true>)
        { return size_t<0>; }

        template <typename Iterable_>
        static constexpr auto length_helper(Iterable_ iterable, Bool<false>)
        { return size_t<1> + length(tail(iterable)); }

        template <typename Iterable_>
        static constexpr auto length_impl(Iterable_ iterable)
        { return length_helper(iterable, is_empty(iterable)); }


        template <typename N, typename Iterable_>
        static constexpr auto drop_helper(N, Iterable_ iterable, Bool<true>)
        { return iterable; }

        template <typename N, typename Iterable_>
        static constexpr auto drop_helper(N n, Iterable_ iterable, Bool<false>)
        { return drop(n - int_<1>, tail(iterable)); }

        template <typename N, typename Iterable_>
        static constexpr auto drop_impl(N n, Iterable_ iterable) {
            return drop_helper(n, iterable,
                                  n == int_<0> || is_empty(iterable));
        }

        // any, all, none
        template <typename Pred, typename Iterable_>
        static constexpr Bool<false>
        any_helper(Pred pred, Iterable_ iterable, Bool<true>)
        { return {}; }

        template <typename Pred, typename Iterable_>
        static constexpr auto
        any_helper(Pred pred, Iterable_ iterable, Bool<false>)
        { return pred(head(iterable)) || any(pred, tail(iterable)); }

        template <typename Pred, typename Iterable_>
        static constexpr auto any_impl(Pred pred, Iterable_ iterable)
        { return any_helper(pred, iterable, is_empty(iterable)); }

        template <typename Pred, typename Iterable_>
        static constexpr auto all_impl(Pred pred, Iterable_ iterable)
        { return !any([=](auto x) { return !pred(x); }, iterable); }

        template <typename Pred, typename Iterable_>
        static constexpr auto none_impl(Pred pred, Iterable_ iterable)
        { return !any(pred, iterable); }


        // any_of, all_of, none_of
        template <typename Iterable_>
        static constexpr auto any_of_impl(Iterable_ iterable)
        { return any([](auto x) { return x; }, iterable); }

        template <typename Iterable_>
        static constexpr auto all_of_impl(Iterable_ iterable)
        { return all([](auto x) { return x; }, iterable); }

        template <typename Iterable_>
        static constexpr auto none_of_impl(Iterable_ iterable)
        { return none([](auto x) { return x; }, iterable); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ITERABLE_HPP
