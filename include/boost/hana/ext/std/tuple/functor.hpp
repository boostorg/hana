/*!
@file
Defines the instance of `boost::hana::Functor` for `std::tuple`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_FUNCTOR_HPP
#define BOOST_HANA_EXT_STD_TUPLE_FUNCTOR_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/ext/std/tuple/tuple.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>

#include <tuple>


namespace boost { namespace hana {
    template <>
    struct Functor::instance<StdTuple> : Functor::fmap_mcd {
        template <typename Xs, typename F, detail::std::size_t ...index>
        static constexpr decltype(auto)
        helper(Xs&& xs, F&& f, detail::std::index_sequence<index...>) {
            return std::make_tuple(
                f(std::get<index>(detail::std::forward<Xs>(xs)))...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) fmap_impl(Xs&& xs, F&& f) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = std::tuple_size<Raw>::value;
            return helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_FUNCTOR_HPP
