/*!
@file
Defines `boost::hana::ext::std::Tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_HPP
#define BOOST_HANA_EXT_STD_TUPLE_HPP

#include <boost/hana/fwd/ext/std/tuple.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/sequence.hpp>

#include <tuple>


#ifdef BOOST_HANA_CONFIG_HAS_NO_STD_TUPLE_ADAPTER
#   error The adapter for std::tuple is not supported with versions of      \
          libc++ prior to the one shipped with Clang 3.7 because of a bug   \
          in the tuple implementation.
#endif

namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::std::Tuple> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return ::std::make_tuple(detail::std::forward<Xs>(xs)...);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<ext::std::Tuple> {
        template <typename Xs, typename F, detail::std::size_t ...index>
        static constexpr decltype(auto)
        transform_helper(Xs&& xs, F&& f, detail::std::index_sequence<index...>) {
            return ::std::make_tuple(
                f(::std::get<index>(detail::std::forward<Xs>(xs)))...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = ::std::tuple_size<Raw>::value;
            return transform_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<ext::std::Tuple> {
        template <typename X>
        static constexpr auto apply(X&& x) {
            return ::std::tuple<typename detail::std::decay<X>::type>{
                                                detail::std::forward<X>(x)};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<ext::std::Tuple> {
        template <typename Xs, detail::std::size_t ...i>
        static constexpr decltype(auto)
        flatten_helper(Xs&& xs, detail::std::index_sequence<i...>) {
            return ::std::tuple_cat(::std::get<i>(
#ifndef BOOST_HANA_CONFIG_LIBCPP_HAS_BUG_22806
                    detail::std::forward<Xs>(xs)
#else
                    xs
#endif
            )...);
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr detail::std::size_t len = ::std::tuple_size<Raw>::value;
            return flatten_helper(detail::std::forward<Xs>(xs),
                    detail::std::make_index_sequence<len>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<ext::std::Tuple> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return ::std::tuple_cat(detail::std::forward<Xs>(xs),
                                    detail::std::forward<Ys>(ys));
        }
    };

    template <>
    struct empty_impl<ext::std::Tuple> {
        static constexpr auto apply()
        { return ::std::tuple<>{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<ext::std::Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::std::get<0>(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct tail_impl<ext::std::Tuple> {
        template <typename Xs, detail::std::size_t ...index>
        static constexpr decltype(auto)
        tail_helper(Xs&& xs, detail::std::index_sequence<index...>) {
            return ::std::make_tuple(
                ::std::get<index + 1>(detail::std::forward<Xs>(xs))...
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using Raw = typename detail::std::remove_reference<Xs>::type;
            constexpr auto N = ::std::tuple_size<Raw>::value;
            return tail_helper(
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<N - 1>{}
            );
        }
    };

    template <>
    struct is_empty_impl<ext::std::Tuple> {
        template <typename ...Xs>
        static constexpr auto apply(::std::tuple<Xs...> const&)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <>
    struct at_impl<ext::std::Tuple> {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N n, Xs&& xs) {
            constexpr detail::std::size_t index = value(n);
            return ::std::get<index>(detail::std::forward<Xs>(xs));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Sequence(ext::std::Tuple)>
        : detail::std::true_type
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_HPP
