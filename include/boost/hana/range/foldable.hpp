/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_FOLDABLE_HPP
#define BOOST_HANA_RANGE_FOLDABLE_HPP

#include <boost/hana/constant.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range/range.hpp>
#include <boost/hana/ring/ring.hpp>


namespace boost { namespace hana {
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
        { return minus(r.to, one<datatype_t<decltype(r.to)>>); }

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
        static constexpr auto sum_impl(R r) {
            using I = datatype_t<decltype(r.from)>;
            constexpr auto from = value(r.from);
            constexpr auto to = value(r.to);
            constexpr auto s = from == to ? 0 : sum_helper(from, to-1);
            return integral_constant<I, decltype(s), s>;
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
            using I = datatype_t<decltype(r.from)>;
            constexpr auto from = value(r.from);
            constexpr auto to = value(r.to);
            constexpr auto s = product_helper(from, to);
            return integral_constant<I, decltype(s), s>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_FOLDABLE_HPP
