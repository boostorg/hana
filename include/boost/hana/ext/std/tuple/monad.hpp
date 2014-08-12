/*!
@file
Defines the instance of `boost::hana::Monad` for `std::tuple`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_MONAD_HPP
#define BOOST_HANA_EXT_STD_TUPLE_MONAD_HPP

#include <boost/hana/ext/std/tuple/tuple.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    template <>
    struct Monad::instance<StdTuple> : Monad::flatten_mcd<StdTuple> {
        template <typename ...Tuples, std::size_t ...Index>
        static constexpr auto
        helper(std::tuple<Tuples...> tuples, std::index_sequence<Index...>)
        { return std::tuple_cat(std::get<Index>(tuples)...); }

        template <typename ...Tuples>
        static constexpr auto flatten_impl(std::tuple<Tuples...> tuples)
        { return helper(tuples, std::index_sequence_for<Tuples...>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_MONAD_HPP
