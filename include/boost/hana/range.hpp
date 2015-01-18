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
#include <boost/hana/constant.hpp> // required by fwd decl
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral.hpp> // required by fwd decl
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(Range)>
        : detail::std::true_type
    { };

    template <>
    struct equal_impl<Range, Range> {
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

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(Range)>
        : detail::std::true_type
    { };

    template <>
    struct unpack_impl<Range> {
        template <typename F, typename From, typename T, T ...vs>
        static constexpr decltype(auto)
        unpack_helper(F&& f, From from, detail::std::integer_sequence<T, vs...>) {
            return detail::std::forward<F>(f)(integral<T, value(from) + vs>...);
        }

        template <typename R, typename F>
        static constexpr decltype(auto) apply(R r, F&& f) {
            auto size = minus(r.to, r.from);
            return unpack_helper(detail::std::forward<F>(f), r.from,
                detail::std::make_index_sequence<value(size)>{});
        }
    };

    template <>
    struct length_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return minus(r.to, r.from); }
    };

    template <>
    struct minimum_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return r.from; }
    };

    template <>
    struct maximum_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return pred(r.to); }
    };

    namespace range_detail {
        template <typename C, typename U>
        struct rebind;

        template <template <typename ...> class C, typename T, typename U>
        struct rebind<C<T>, U> { using type = C<U>; };
    }

    template <>
    struct sum_impl<Range> {
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

        template <typename R>
        static constexpr auto apply(R r) {
            using C = datatype_t<decltype(r.from)>;
            constexpr auto from = value(r.from);
            constexpr auto to = value(r.to);
            constexpr auto s = from == to ? 0 : sum_helper(from, to-1);
            return integral_constant<
                typename range_detail::rebind<C, decltype(s)>::type, s
            >();
        }
    };

    template <>
    struct product_impl<Range> {
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
        static constexpr auto apply(R r) {
            using C = datatype_t<decltype(r.from)>;
            constexpr auto from = value(r.from);
            constexpr auto to = value(r.to);
            constexpr auto s = product_helper(from, to);
            return integral_constant<
                typename range_detail::rebind<C, decltype(s)>::type, s
            >();
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Iterable(Range)>
        : detail::std::true_type
    { };

    template <>
    struct head_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return r.from; }
    };

    template <>
    struct tail_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return range(succ(r.from), r.to); }
    };

    template <>
    struct is_empty_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return equal(r.from, r.to); }
    };

    template <>
    struct at_impl<Range> {
        template <typename N, typename R>
        static constexpr auto apply(N n, R r)
        { return plus(r.from, n); }
    };

    template <>
    struct last_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r)
        { return pred(r.to); }
    };

    template <>
    struct drop_impl<Range> {
        template <typename N, typename R>
        static constexpr auto apply(N n, R r)
        { return range(min(r.to, plus(r.from, n)), r.to); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
