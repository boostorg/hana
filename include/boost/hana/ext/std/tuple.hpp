/*!
@file
Defines `boost::hana::ext::std::Tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_HPP
#define BOOST_HANA_EXT_STD_TUPLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>


#ifdef BOOST_HANA_CONFIG_HAS_NO_STD_TUPLE_ADAPTER
#   error The adapter for std::tuple is not supported with versions of      \
          libc++ prior to the one shipped with Clang 3.7 because of a bug   \
          in the tuple implementation.
#endif

namespace boost { namespace hana {
    namespace ext { namespace std { struct Tuple; }}

    template <typename ...Xs>
    struct datatype<std::tuple<Xs...>> {
        using type = ext::std::Tuple;
    };

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::std::Tuple> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return std::make_tuple(static_cast<Xs&&>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<ext::std::Tuple> {
        template <typename X>
        static constexpr auto apply(X&& x) {
            return std::tuple<typename std::decay<X>::type>{
                                                static_cast<X&&>(x)};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<ext::std::Tuple> {
        template <typename Xs, std::size_t ...i>
        static constexpr decltype(auto)
        flatten_helper(Xs&& xs, std::index_sequence<i...>) {
            return std::tuple_cat(std::get<i>(
#ifndef BOOST_HANA_CONFIG_LIBCPP_HAS_BUG_22806
                    static_cast<Xs&&>(xs)
#else
                    xs
#endif
            )...);
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs xs) {
            using Raw = typename std::remove_reference<Xs>::type;
            constexpr std::size_t len = std::tuple_size<Raw>::value;
            return flatten_helper(xs,
                    std::make_index_sequence<len>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct empty_impl<ext::std::Tuple> {
        static constexpr auto apply()
        { return std::tuple<>{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct front_impl<ext::std::Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return std::get<0>(static_cast<Xs&&>(xs));
        }
    };

    template <>
    struct tail_impl<ext::std::Tuple> {
        template <typename Xs, std::size_t ...index>
        static constexpr decltype(auto)
        tail_helper(Xs&& xs, std::index_sequence<index...>) {
            return std::make_tuple(
                std::get<index + 1>(static_cast<Xs&&>(xs))...
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using Raw = typename std::remove_reference<Xs>::type;
            constexpr auto N = std::tuple_size<Raw>::value;
            return tail_helper(
                static_cast<Xs&&>(xs),
                std::make_index_sequence<N - 1>{}
            );
        }
    };

    template <>
    struct is_empty_impl<ext::std::Tuple> {
        template <typename ...Xs>
        static constexpr auto apply(std::tuple<Xs...> const&)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <>
    struct at_impl<ext::std::Tuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t index = hana::value<N>();
            return std::get<index>(static_cast<Xs&&>(xs));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, ext::std::Tuple>
        : decltype(true_)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_HPP
