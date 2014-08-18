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
#include <boost/hana/ext/std/tuple/tuple.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<StdTuple> : Iterable::mcd {
        template <typename ...Xs>
        static constexpr auto head_impl(std::tuple<Xs...> tuple)
        { return std::get<0>(tuple); }

        template <typename ...Xs, std::size_t ...Index>
        static constexpr auto
        tail_helper(std::tuple<Xs...> tuple, std::index_sequence<Index...>) {
            //! @todo
            //! Use `std::make_tuple` when this bug is fixed:
            //! http://llvm.org/bugs/show_bug.cgi?id=19793
            using T = std::tuple<
                std::tuple_element_t<Index + 1, std::tuple<Xs...>>...
            >;
            T tmp{std::get<Index + 1>(tuple)...};
            return tmp;
        }

        template <typename ...Xs>
        static constexpr auto tail_impl(std::tuple<Xs...> tuple)
        { return tail_helper(tuple, std::make_index_sequence<sizeof...(Xs) - 1>{}); }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(std::tuple<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_ITERABLE_HPP
