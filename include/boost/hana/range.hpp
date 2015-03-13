/*!
@file
Defines `boost::hana::Range`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_HPP
#define BOOST_HANA_RANGE_HPP

#include <boost/hana/fwd/range.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral_constant.hpp> // required by fwd decl and below
#include <boost/hana/iterable.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _range
    //////////////////////////////////////////////////////////////////////////
    template <typename IntegralConstant,
              typename IntegralConstant::value_type From,
              typename IntegralConstant::value_type To>
    struct _range
        : operators::adl
        , operators::Iterable_ops<_range<IntegralConstant, From, To>>
    {
        using underlying = IntegralConstant;
        static_assert(From <= To,
        "invalid usage of boost::hana::range(from, to) with from > to");

        static constexpr auto from = From;
        static constexpr auto to = To;
    };

    template <typename IntegralConstant,
              typename IntegralConstant::value_type From,
              typename IntegralConstant::value_type To>
    struct datatype<_range<IntegralConstant, From, To>> {
        using type = Range;
    };

    //////////////////////////////////////////////////////////////////////////
    // make<Range>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Range> {
        template <typename From, typename To>
        static constexpr auto apply(From from, To to) {
            using C = typename common<typename datatype<From>::type,
                                      typename datatype<To>::type>::type;
            auto cfrom = hana::to<C>(from);
            auto cto = hana::to<C>(to);
            return _range<C, hana::value(cfrom), hana::value(cto)>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Range>
        : operators::of<Comparable, Iterable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Range, Range> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1, R2) {
            return bool_<
                (R1::from == R1::to && R2::from == R2::to) ||
                (R1::from == R2::from && R1::to == R2::to)
            >;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Range> {
        template <typename R, typename F, typename T, T ...v>
        static constexpr decltype(auto)
        unpack_helper(R r, F&& f, detail::std::integer_sequence<T, v...>) {
            using U = typename R::underlying;
            return detail::std::forward<F>(f)(
                to<U>(integral_constant<T, R::from + v>)...
            );
        }

        template <typename R, typename F>
        static constexpr decltype(auto) apply(R r, F&& f) {
            constexpr auto size = R::to - R::from;
            return unpack_helper(r, detail::std::forward<F>(f),
                detail::std::make_integer_sequence<decltype(size), size>{});
        }
    };

    template <>
    struct length_impl<Range> {
        template <typename R>
        static constexpr auto apply(R)
        { return size_t<R::to - R::from>; }
    };

    template <>
    struct minimum_impl<Range> {
        template <typename R>
        static constexpr auto apply(R) {
            using U = typename R::underlying;
            return hana::to<U>(integral_constant<decltype(R::from), R::from>);
        }
    };

    template <>
    struct maximum_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r) {
            using U = typename R::underlying;
            return hana::to<U>(integral_constant<decltype(R::to-1), R::to-1>);
        }
    };

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
        static constexpr auto apply(R) {
            using U = typename R::underlying;
            constexpr auto from = R::from;
            constexpr auto to = R::to;
            constexpr auto s = from == to ? 0 : sum_helper(from, to-1);
            return hana::to<U>(integral_constant<decltype(s), s>);
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
        static constexpr auto apply(R) {
            using U = typename R::underlying;
            constexpr auto p = product_helper(R::from, R::to);
            return hana::to<U>(integral_constant<decltype(p), p>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<Range>
        : Iterable::find_if_impl<Range>
    { };

    template <>
    struct any_of_impl<Range>
        : Iterable::any_of_impl<Range>
    { };

    template <>
    struct find_impl<Range> {
        template <typename U, typename N>
        static constexpr auto find_helper(N n_, decltype(true_)) {
            constexpr auto n = hana::value(n_);
            return hana::just(hana::to<U>(integral_constant<decltype(n), n>));
        }

        template <typename U, typename N>
        static constexpr auto find_helper(N, decltype(false_))
        { return nothing; }

        template <typename R, typename N>
        static constexpr auto apply(R, N n_) {
            constexpr auto n = hana::value(n_);
            return find_helper<typename R::underlying>(
                                    n_, bool_<(n >= R::from && n < R::to)>);
        }
    };

    template <>
    struct elem_impl<Range> {
        template <typename R, typename N>
        static constexpr auto apply(R, N n_) {
            constexpr auto n = hana::value(n_);
            return bool_<(n >= R::from && n < R::to)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<Range> {
        template <typename R>
        static constexpr auto apply(R) {
            using U = typename R::underlying;
            constexpr auto from = R::from;
            return hana::to<U>(integral_constant<decltype(from), from>);
        }
    };

    template <>
    struct tail_impl<Range> {
        template <typename R>
        static constexpr auto apply(R) {
            using U = typename R::underlying;
            return _range<U, R::from + 1, R::to>{};
        }
    };

    template <>
    struct is_empty_impl<Range> {
        template <typename R>
        static constexpr auto apply(R)
        { return bool_<R::from == R::to>; }
    };

    template <>
    struct at_impl<Range> {
        template <typename N, typename R>
        static constexpr auto apply(N n_, R r) {
            using U = typename R::underlying;
            constexpr auto n = hana::value(n_);
            constexpr auto from = R::from;
            return to<U>(integral_constant<decltype(from + n), from + n>);
        }
    };

    template <>
    struct last_impl<Range> {
        template <typename R>
        static constexpr auto apply(R r) {
            using U = typename R::underlying;
            constexpr auto to = R::to;
            return hana::to<U>(integral_constant<decltype(to-1), to-1>);
        }
    };

    template <>
    struct drop_impl<Range> {
        template <typename N, typename R>
        static constexpr auto apply(N n_, R) {
            constexpr auto n = hana::value(n_);
            using U = typename R::underlying;
            return _range<U,
                (R::to < R::from + n ? R::to : R::from + n),
                R::to
            >{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
