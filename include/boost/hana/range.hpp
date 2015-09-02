/*!
@file
Defines `boost::hana::range`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_HPP
#define BOOST_HANA_RANGE_HPP

#include <boost/hana/fwd/range.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/back.hpp>
#include <boost/hana/fwd/contains.hpp>
#include <boost/hana/fwd/drop_front.hpp>
#include <boost/hana/fwd/drop_front_exactly.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/find.hpp>
#include <boost/hana/fwd/front.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/maximum.hpp>
#include <boost/hana/fwd/minimum.hpp>
#include <boost/hana/fwd/product.hpp>
#include <boost/hana/fwd/sum.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/integral_constant.hpp> // required by fwd decl and below
#include <boost/hana/optional.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // range<>
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T, T From, T To>
    struct range
        : operators::adl
        , detail::iterable_operators<range<T, From, To>>
    {
        static_assert(From <= To,
        "invalid usage of boost::hana::make_range(from, to) with from > to");

        using value_type = T;
        static constexpr value_type from = From;
        static constexpr value_type to = To;
    };
    //! @endcond

    template <typename T, T From, T To>
    struct datatype<range<T, From, To>> {
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
            static_assert(_models<Constant, From>::value,
            "hana::make<Range>(from, to) requires 'from' to be a Constant");

            static_assert(_models<Constant, To>::value,
            "hana::make<Range>(from, to) requires 'to' to be a Constant");
        #endif

            using T = typename common<
                typename datatype<From>::type::value_type,
                typename datatype<To>::type::value_type
            >::type;
            constexpr T from = hana::to<T>(hana::value<From>());
            constexpr T to = hana::to<T>(hana::value<To>());
            return range<T, from, to>{};
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
            return hana::bool_c<
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
            return static_cast<F&&>(f)(integral_constant<T, from + v>{}...);
        }

        template <typename T, T from, T to, typename F>
        static constexpr decltype(auto) apply(range<T, from, to> const&, F&& f) {
            return unpack_helper<T, from>(static_cast<F&&>(f),
                std::make_integer_sequence<T, to - from>{});
        }
    };

    template <>
    struct length_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return hana::size_c<static_cast<std::size_t>(to - from)>; }
    };

    template <>
    struct minimum_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return integral_c<T, from>; }
    };

    template <>
    struct maximum_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return integral_c<T, to-1>; }
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
        static constexpr auto apply(range<T, from, to> const&) {
            return integral_c<T, from == to ? 0 : sum_helper(from, to-1)>;
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
        static constexpr auto apply(range<T, from, to> const&)
        { return integral_c<T, product_helper(from, to)>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<Range> {
        template <typename T, T from, typename N>
        static constexpr auto find_helper(decltype(hana::true_c)) {
            constexpr auto n = static_cast<T>(hana::value<N>());
            return hana::just(integral_c<T, n>);
        }

        template <typename T, T from, typename N>
        static constexpr auto find_helper(decltype(hana::false_c))
        { return nothing; }

        template <typename T, T from, T to, typename N>
        static constexpr auto apply(range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return find_helper<T, from, N>(bool_c<(n >= from && n < to)>);
        }
    };

    template <>
    struct contains_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return bool_c<(n >= from && n < to)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct front_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return integral_c<T, from>; }
    };

    template <>
    struct tail_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return range<T, from + 1, to>{}; }
    };

    template <>
    struct is_empty_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return bool_c<from == to>; }
    };

    template <>
    struct at_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return integral_c<T, from + n>;
        }
    };

    template <>
    struct back_impl<Range> {
        template <typename T, T from, T to>
        static constexpr auto apply(range<T, from, to> const&)
        { return integral_c<T, to - 1>; }
    };

    template <>
    struct drop_front_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return range<T, (to < from + n ? to : from + n), to>{};
        }
    };

    template <>
    struct drop_front_exactly_impl<Range> {
        template <typename T, T from, T to, typename N>
        static constexpr auto apply(range<T, from, to> const&, N const&) {
            constexpr auto n = hana::value<N>();
            return range<T, from + n, to>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_HPP
