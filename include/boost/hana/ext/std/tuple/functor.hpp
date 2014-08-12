/*!
@file
Defines the instance of `boost::hana::Functor` for `std::tuple`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_FUNCTOR_HPP
#define BOOST_HANA_EXT_STD_TUPLE_FUNCTOR_HPP

#include <boost/hana/ext/std/tuple/tuple.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>

#include <cstddef>
#include <tuple>
#include <utility>


namespace boost { namespace hana {
    template <>
    struct Functor::instance<StdTuple> : Functor::fmap_mcd {
        template <typename F, typename ...Xs, std::size_t ...Index>
        static constexpr auto
        helper(F f, std::tuple<Xs...> tuple, std::index_sequence<Index...>)
        { return std::make_tuple(f(std::get<Index>(tuple))...); }

        template <typename F, typename ...Xs>
        static constexpr auto fmap_impl(F f, std::tuple<Xs...> tuple)
        { return helper(f, tuple, std::index_sequence_for<Xs...>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_FUNCTOR_HPP
