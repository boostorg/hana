/*!
@file
Defines `boost::hana::basic_tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BASIC_TUPLE_HPP
#define BOOST_HANA_BASIC_TUPLE_HPP

#include <boost/hana/fwd/basic_tuple.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/unpack.hpp>

#if 0 //! @todo Until we strip down headers, this includes too much
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/bool.hpp>
#include <boost/hana/fwd/concept/sequence.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/integral_constant.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/value.hpp>
#endif

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
#if defined(BOOST_HANA_CONFIG_CLANG)
#   if __has_extension(is_empty) && __has_extension(is_final)
#       define BOOST_HANA_HAS_TUPLE_ELEMENT_INTRINSICS
#   endif
#endif

    namespace detail {
        //////////////////////////////////////////////////////////////////////
        // elt<n, Xn>
        //
        // `elt` stands for `tuple_element`; the name is compressed to reduce
        // symbol lengths.
        //
        // Wrapper holding the actual elements of a tuple. It takes care of
        // optimizing the storage for empty types.
        //
        // When available, we use compiler intrinsics to reduce the number
        // of instantiations.
        //////////////////////////////////////////////////////////////////////
        template <std::size_t n, typename Xn, bool =
#if defined(BOOST_HANA_HAS_TUPLE_ELEMENT_INTRINSICS)
            __is_empty(Xn) && !__is_final(Xn)
#else
            std::is_empty<Xn>::value && !std::is_final<Xn>::value
#endif
        >
        struct elt;

        // Specialize storage for empty types
        template <std::size_t n, typename Xn>
        struct elt<n, Xn, true> : Xn {
            constexpr elt() = default;

            template <typename Yn>
            explicit constexpr elt(Yn&& yn)
                : Xn(static_cast<Yn&&>(yn))
            { }
        };

        // Specialize storage for non-empty types
        template <std::size_t n, typename Xn>
        struct elt<n, Xn, false> {
            constexpr elt() = default;

            template <typename Yn>
            explicit constexpr elt(Yn&& yn)
                : data_(static_cast<Yn&&>(yn))
            { }

            Xn data_;
        };

        //////////////////////////////////////////////////////////////////////
        // basic_tuple_impl<n, Xn>
        //////////////////////////////////////////////////////////////////////
        template <typename Indices, typename ...Xn>
        struct basic_tuple_impl;

        template <std::size_t ...n, typename ...Xn>
        struct basic_tuple_impl<std::index_sequence<n...>, Xn...>
            : detail::elt<n, Xn>...
        {
            constexpr basic_tuple_impl() = default;

            template <typename ...Yn>
            explicit constexpr basic_tuple_impl(Yn&& ...yn)
                : detail::elt<n, Xn>(static_cast<Yn&&>(yn))...
            { }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // basic_tuple
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename ...Xn>
    struct basic_tuple final
        : detail::basic_tuple_impl<std::make_index_sequence<sizeof...(Xn)>, Xn...>
    {
        using detail::basic_tuple_impl<
            std::make_index_sequence<sizeof...(Xn)>, Xn...
        >::basic_tuple_impl;
    };
    //! @endcond

    template <typename ...Xn>
    struct datatype<basic_tuple<Xn...>> {
        using type = BasicTuple;
    };

    //////////////////////////////////////////////////////////////////////////
    // get_impl
    //////////////////////////////////////////////////////////////////////////
    template <std::size_t n, typename Xn>
    constexpr Xn const& get_impl(detail::elt<n, Xn, true> const& xn)
    { return xn; }

    template <std::size_t n, typename Xn>
    constexpr Xn& get_impl(detail::elt<n, Xn, true>& xn)
    { return xn; }

    template <std::size_t n, typename Xn>
    constexpr Xn&& get_impl(detail::elt<n, Xn, true>&& xn)
    { return static_cast<Xn&&>(xn); }


    template <std::size_t n, typename Xn>
    constexpr Xn const& get_impl(detail::elt<n, Xn, false> const& xn)
    { return xn.data_; }

    template <std::size_t n, typename Xn>
    constexpr Xn& get_impl(detail::elt<n, Xn, false>& xn)
    { return xn.data_; }

    template <std::size_t n, typename Xn>
    constexpr Xn&& get_impl(detail::elt<n, Xn, false>&& xn)
    { return static_cast<Xn&&>(xn.data_); }


    //////////////////////////////////////////////////////////////////////////
    // make<BasicTuple>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<BasicTuple> {
        template <typename ...Xn>
        static constexpr basic_tuple<typename std::decay<Xn>::type...>
        apply(Xn&& ...xn) {
            return basic_tuple<typename std::decay<Xn>::type...>{
                static_cast<Xn&&>(xn)...
            };
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unpack
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<BasicTuple> {
        template <std::size_t ...i, typename ...Xn, typename F>
        static constexpr decltype(auto)
        apply(detail::basic_tuple_impl<std::index_sequence<i...>, Xn...> const& xs, F&& f) {
            return static_cast<F&&>(f)(
                get_impl<i>(static_cast<detail::elt<i, Xn> const&>(xs))...
            );
        }

        template <std::size_t ...i, typename ...Xn, typename F>
        static constexpr decltype(auto)
        apply(detail::basic_tuple_impl<std::index_sequence<i...>, Xn...>& xs, F&& f) {
            return static_cast<F&&>(f)(
                get_impl<i>(static_cast<detail::elt<i, Xn>&>(xs))...
            );
        }

        template <std::size_t ...i, typename ...Xn, typename F>
        static constexpr decltype(auto)
        apply(detail::basic_tuple_impl<std::index_sequence<i...>, Xn...>&& xs, F&& f) {
            return static_cast<F&&>(f)(
                get_impl<i>(static_cast<detail::elt<i, Xn>&&>(xs))...
            );
        }
    };

#if 0
    //////////////////////////////////////////////////////////////////////////
    // length
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct length_impl<BasicTuple> {
        template <typename ...Xn>
        static constexpr auto apply(basic_tuple<Xn...> const&) {
            return hana::size_c<sizeof...(Xn)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models<Sequence>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, BasicTuple> {
        static constexpr bool value = true;
    };

    //////////////////////////////////////////////////////////////////////////
    // at
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<BasicTuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t index = hana::value<N>();
            return hana::get_impl<index>(static_cast<Xs&&>(xs));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // tail
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct tail_impl<BasicTuple> {
        template <typename Xs, std::size_t ...i>
        static constexpr auto tail_helper(Xs&& xs, std::index_sequence<0, i...>) {
            return hana::make_basic_tuple(hana::get_impl<i>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            constexpr std::size_t N = decltype(hana::length(xs))::value;
            return tail_helper(static_cast<Xs&&>(xs),
                               std::make_index_sequence<N>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // is_empty
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct is_empty_impl<BasicTuple> {
        template <typename ...Xs>
        static constexpr auto apply(basic_tuple<Xs...> const&)
        { return hana::bool_c<sizeof...(Xs) == 0>; }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_BASIC_TUPLE_HPP
