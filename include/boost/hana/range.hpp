/*!
@file
Defines `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_HPP
#define BOOST_HANA_RANGE_HPP

#include <boost/hana/fwd/range.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Range`s.
    //!
    //! Two ranges are equal if and only if they are both empty or they have
    //! the same `head` and the same length.
    template <typename _>
    struct equal_impl<Range, Range, _> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1 r1, R2 r2) {
            return or_(
                and_(is_empty(r1), is_empty(r2)),
                and_(
                    equal(r1.from, r2.from),
                    equal(r1.to, r2.to)
                )
            );
        }
    };

    //! Instance of `Foldable` for `Range`s.
    template <>
    struct Foldable::instance<Range> : Foldable::unpack_mcd {
        template <typename F, typename From, typename T, T ...vs>
        static constexpr decltype(auto)
        unpack_helper(F&& f, From from, detail::std::integer_sequence<T, vs...>) {
            return detail::std::forward<F>(f)(integral<T, from() + vs>...);
        }

        template <typename R, typename F>
        static constexpr decltype(auto) unpack_impl(R r, F&& f) {
            auto size = minus(r.to, r.from);
            return unpack_helper(detail::std::forward<F>(f), r.from,
                detail::std::make_index_sequence<size()>{});
        }

        template <typename R>
        static constexpr auto length_impl(R r)
        { return minus(r.to, r.from); }

        template <typename R>
        static constexpr auto minimum_impl(R r)
        { return r.from; }

        template <typename R>
        static constexpr auto maximum_impl(R r)
        { return pred(r.to); }

        // Returns the sum of `[m, n]`, where `m <= n` always hold.
        template <typename I>
        static constexpr I sum_helper(I m, I n) {
            if (m == n)
                return m;

            // 0 == m < n
            else if (0 == m)
                return n * (n+1) / 2;

            // 0 < m < n
            else if (0 < m)
                return sum_helper(0, n) - sum_helper(0, m-1);

            // m < 0 <= n
            else if (0 <= n)
                return sum_helper(0, n) - sum_helper(0, -m);

            // m < n < 0
            else
                return -sum_helper(-n, -m);
        }

        template <typename C, typename U>
        struct rebind;

        template <template <typename ...> class C, typename T, typename U>
        struct rebind<C<T>, U> { using type = C<U>; };

        template <typename R>
        static constexpr auto sum_impl(R r) {
            using C = datatype_t<decltype(r.from)>;
            constexpr auto from = value(r.from);
            constexpr auto to = value(r.to);
            constexpr auto s = from == to ? 0 : sum_helper(from, to-1);
            return integral_constant<
                typename rebind<C, decltype(s)>::type, s
            >();
        }

        // Returns the product of `[m, n)`, where `m <= n` always hold.
        template <typename I>
        static constexpr I product_helper(I m, I n) {
            if (m <= 0 && 0 < n)
                return 0;
            else {
                I p = 1;
                for (; m != n; ++m)
                    p *= m;
                return p;
            }
        }

        template <typename R>
        static constexpr auto product_impl(R r) {
            using C = datatype_t<decltype(r.from)>;
            constexpr auto from = value(r.from);
            constexpr auto to = value(r.to);
            constexpr auto s = product_helper(from, to);
            return integral_constant<
                typename rebind<C, decltype(s)>::type, s
            >();
        }
    };

    //! Instance of `Iterable` for `Range`s.
    //!
    //! Let `r` be a `Range` containing the `Integral`s in the half-open
    //! interval `[from, to)`. The head of `r` is an `Integral` with value
    //! `from`, its tail is the range representing the `[from + 1, to)`
    //! interval and `r` is empty if and only if `from == to`.
    //!
    //! @snippet example/range.cpp iterable
    //!
    //! @todo
    //! Consider allowing the elements of the range to be any `Group`, since
    //! we don't use much more than that (except `Comparable` and `Orderable`).
    template <>
    struct Iterable::instance<Range> : Iterable::mcd {
        template <typename R>
        static constexpr auto head_impl(R r)
        { return r.from; }

        //! @todo
        //! We need a type class to express the concept of
        //! incrementing and decrementing.
        template <typename R>
        static constexpr auto tail_impl(R r)
        { return range(succ(r.from), r.to); }

        template <typename R>
        static constexpr auto is_empty_impl(R r)
        { return equal(r.from, r.to); }

        template <typename N, typename R>
        static constexpr auto at_impl(N n, R r)
        { return plus(r.from, n); }

        template <typename R>
        static constexpr auto last_impl(R r)
        { return pred(r.to); }

        template <typename N, typename R>
        static constexpr auto drop_impl(N n, R r) {
            return range(min(r.to, plus(r.from, n)), r.to);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
