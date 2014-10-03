/*!
@file
Defines the instance of `boost::hana::Iterable` for `std::tuple`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_ITERABLE_HPP
#define BOOST_HANA_EXT_STD_TUPLE_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/ext/std/tuple/tuple.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <tuple>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<StdTuple> : Iterable::mcd {
        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return std::get<0>(detail::std::forward<Xs>(xs));
        }

        template <typename Xs, detail::std::size_t ...index>
        static constexpr decltype(auto)
        tail_helper(Xs&& xs, detail::std::index_sequence<index...>) {
            //! @todo
            //! Use `std::make_tuple` when this bug is fixed:
            //! http://llvm.org/bugs/show_bug.cgi?id=19793
            using Raw = typename detail::std::remove_reference<Xs>::type;
            using T = std::tuple<std::tuple_element_t<index + 1, Raw>...>;
            T tmp{std::get<index + 1>(detail::std::forward<Xs>(xs))...};
            return tmp;
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = std::tuple_size<Raw>::value;
            return tail_helper(
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<N - 1>{}
            );
        }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(std::tuple<Xs...> const&)
        { return bool_<sizeof...(Xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_ITERABLE_HPP
