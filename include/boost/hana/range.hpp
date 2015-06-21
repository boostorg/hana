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
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral_constant.hpp> // required by fwd decl and below
#include <boost/hana/iterable.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/searchable.hpp>

#include <cstddef>
#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _range
    //////////////////////////////////////////////////////////////////////////
    template <typename T, T From, T To>
    struct _range
        : operators::adl
        , detail::iterable_operators<_range<T, From, To>>
    {
        static_assert(From <= To,
        "invalid usage of boost::hana::range(from, to) with from > to");

        using value_type = T;
        static constexpr value_type from = From;
        static constexpr value_type to = To;
    };

    template <typename T, T From, T To>
    struct datatype<_range<T, From, To>> {
        using type = Range;
    };

    //////////////////////////////////////////////////////////////////////////
    // make<Range>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Range> {
        template <typename From, typename To>
        static constexpr auto apply(From const&, To const&) {

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Constant, From>{}(),
            "hana::make<Range>(from, to) requires 'from' to be a Constant");

            static_assert(_models<Constant, To>{}(),
            "hana::make<Range>(from, to) requires 'to' to be a Constant");
        #endif

            using T = typename common<
                typename datatype<From>::type::value_type,
                typename datatype<To>::type::value_type
            >::type;
            constexpr T from = hana::to<T>(hana::value<From>());
            constexpr T to = hana::to<T>(hana::value<To>());
            return _range<T, from, to>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<Range> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Range, Range> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1 const&, R2 const&) {
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
        template <typename T, T from, typename F, T ...v>
        static constexpr decltype(auto)
        unpack_helper(F&& f, std::integer_sequence<T, v...>) {
            return static_cast<F&&>(f)(_integral_constant<T, from + v>{}...);
        }

        template <typename T, T from, T to, typename F>
        static constexpr decltype(auto) apply(_range<T, from, to> const&, F&& f) {
            return unpack_helper<T, from>(static_cast<F&&>(f),
                std::make_integer_sequence<T, to - from>{});
        }
    };

    template <>
    struct length_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return size_t<static_cast<std::size_t>(to - from)>; }
    };

    template <>
    struct minimum_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return integral_constant<T, from>; }
    };

    template <>
    struct maximum_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return integral_constant<T, to-1>; }
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

        template <typename, typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&) {
            return integral_constant<T, from == to ? 0 : sum_helper(from, to-1)>;
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

        template <typename, typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return integral_constant<T, product_helper(from, to)>; }
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
        template <typename T, T from, typename N>
        static constexpr auto find_helper(decltype(true_)) {
            constexpr auto n = static_cast<T>(hana::value<N>());
            return hana::just(integral_constant<T, n>);
        }

        template <typename T, T from, typename N>
        static constexpr auto find_helper(decltype(false_))
        { return nothing; }

        template <typename T, T from, T to, typename N>
        static constexpr auto apply(_range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return find_helper<T, from, N>(bool_<(n >= from && n < to)>);
        }
    };

    template <>
    struct contains_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(_range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return bool_<(n >= from && n < to)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return integral_constant<T, from>; }
    };

    template <>
    struct tail_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return _range<T, from + 1, to>{}; }
    };

    template <>
    struct is_empty_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return bool_<from == to>; }
    };

    template <>
    struct at_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(_range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return integral_constant<T, from + n>;
        }
    };

    template <>
    struct last_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(_range<T, from, to> const&)
        { return integral_constant<T, to - 1>; }
    };

    template <>
    struct drop_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(_range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return _range<T, (to < from + n ? to : from + n), to>{};
        }
    };

    template <>
    struct drop_exactly_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(_range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return _range<T, from + n, to>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
