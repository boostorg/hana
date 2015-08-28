/*!
@file
Defines `boost::hana::tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_HPP
#define BOOST_HANA_TUPLE_HPP

#include <boost/hana/fwd/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/detail/operators/monad.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/fwd/unpack.hpp>
#include <boost/hana/type.hpp> // required by fwd decl of tuple_t
#include <boost/hana/value.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // tuple
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename ...Xs>
    struct tuple
        : detail::closure<Xs...>
        , operators::adl
        , detail::iterable_operators<tuple<Xs...>>
    {
        using detail::closure<Xs...>::closure; // inherit constructors
        tuple(tuple const&) = default;
        tuple(tuple&&) = default;
        tuple() = default;
        tuple(tuple&) = default;

        using hana = tuple;
        using datatype = Tuple;
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<Tuple> {
            static constexpr bool value = true;
        };
        template <>
        struct orderable_operators<Tuple> {
            static constexpr bool value = true;
        };
        template <>
        struct monad_operators<Tuple> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Tuple> {
        template <typename Xs, typename F, std::size_t ...i>
        static constexpr decltype(auto)
        unpack_helper(Xs&& xs, F&& f, std::index_sequence<i...>) {
            return static_cast<F&&>(f)(hana::at_c<i>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            constexpr std::size_t N = hana::value<decltype(hana::length(xs))>();
            return unpack_helper(static_cast<Xs&&>(xs), static_cast<F&&>(f),
                                 std::make_index_sequence<N>{});
        }
    };

    template <>
    struct length_impl<Tuple> {
        template <typename ...Xs>
        static constexpr auto apply(tuple<Xs...> const&)
        { return hana::size_t<sizeof...(Xs)>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<Tuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t index = hana::value<N>();
            return detail::get<index>(static_cast<Xs&&>(xs));
        }
    };

    template <>
    struct tail_impl<Tuple> {
        template <typename Xs, std::size_t ...i>
        static constexpr auto tail_helper(Xs&& xs, std::index_sequence<0, i...>) {
            return hana::make<Tuple>(hana::at_c<i>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            constexpr std::size_t N = hana::value<decltype(hana::length(xs))>();
            return tail_helper(static_cast<Xs&&>(xs),
                               std::make_index_sequence<N>{});
        }
    };

    template <>
    struct is_empty_impl<Tuple> {
        template <typename ...Xs>
        static constexpr auto apply(tuple<Xs...> const&)
        { return hana::bool_<sizeof...(Xs) == 0>; }
    };

    // compile-time optimizations (to reduce the # of function instantiations)
    template <std::size_t n, typename ...Xs>
    constexpr decltype(auto) at_c(tuple<Xs...> const& xs) {
        return detail::get<n>(xs);
    }

    template <std::size_t n, typename ...Xs>
    constexpr decltype(auto) at_c(tuple<Xs...>& xs) {
        return detail::get<n>(xs);
    }

    template <std::size_t n, typename ...Xs>
    constexpr decltype(auto) at_c(tuple<Xs...>&& xs) {
        return detail::get<n>(static_cast<tuple<Xs...>&&>(xs));
    }

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, Tuple> {
        static constexpr bool value = true;
    };

    template <>
    struct make_impl<Tuple> {
        template <typename ...Xs>
        static constexpr
        tuple<typename std::decay<Xs>::type...> apply(Xs&& ...xs)
        { return {static_cast<Xs&&>(xs)...}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_HPP
