/*!
@file
Defines the instance of `boost::hana::List` for `std::array`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_ARRAY_LIST_HPP
#define BOOST_HANA_EXT_STD_ARRAY_LIST_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/ext/std/array/foldable.hpp>
#include <boost/hana/ext/std/array/iterable.hpp>
#include <boost/hana/ext/std/array/monad.hpp>
#include <boost/hana/list/mcd.hpp>

#include <array>


namespace boost { namespace hana {
    template <>
    struct List::instance<StdArray> : List::mcd<StdArray> {
        struct anything { };

        static constexpr auto nil_impl()
        { return std::array<anything, 0>{}; }

        template <typename T, detail::std::size_t N, typename X, typename Xs, detail::std::size_t ...index>
        static constexpr auto
        cons_helper(X&& x, Xs&& xs, detail::std::index_sequence<index...>) {
            return std::array<T, N + 1>{{
                detail::std::forward<X>(x),
                detail::std::forward<Xs>(xs)[index]...
            }};
        }

        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            using RawArray = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = std::tuple_size<RawArray>::value;
            using T = typename RawArray::value_type;
            return cons_helper<T, N>(
                detail::std::forward<X>(x),
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<N>{}
            );
        }

        template <typename X>
        static constexpr auto cons_impl(X x, std::array<anything, 0>)
        { return std::array<X, 1>{{detail::std::move(x)}}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_ARRAY_LIST_HPP
