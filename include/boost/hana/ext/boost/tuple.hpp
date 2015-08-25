/*!
@file
Defines `boost::hana::ext::boost::Tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/value.hpp>

#include <boost/tuple/tuple.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace boost { struct Tuple; }}

    template <typename ...Xs>
    struct datatype< ::boost::tuple<Xs...>> {
        using type = ext::boost::Tuple;
    };

    template <typename H, typename T>
    struct datatype< ::boost::tuples::cons<H, T>> {
        using type = ext::boost::Tuple;
    };

    template <>
    struct datatype< ::boost::tuples::null_type> {
        using type = ext::boost::Tuple;
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<ext::boost::Tuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t n = hana::value<N>();
            return static_cast<Xs&&>(xs).template get<n>();
        }
    };

    template <>
    struct tail_impl<ext::boost::Tuple> {
        template <typename H>
        static constexpr auto
        apply(::boost::tuples::cons<H, ::boost::tuples::null_type> const&)
        { return ::boost::tuples::null_type{}; }

        template <typename H>
        static constexpr auto
        apply(::boost::tuples::cons<H, ::boost::tuples::null_type>&)
        { return ::boost::tuples::null_type{}; }

        template <typename H, typename T>
        static constexpr auto const& apply(::boost::tuples::cons<H, T> const& xs)
        { return xs.get_tail(); }

        template <typename H, typename T>
        static constexpr auto& apply(::boost::tuples::cons<H, T>& xs)
        { return xs.get_tail(); }

        template <typename H, typename T>
        static constexpr auto apply(::boost::tuples::cons<H, T>&& xs)
        { return std::move(xs).get_tail(); }
    };

    template <>
    struct is_empty_impl<ext::boost::Tuple> {
        static constexpr auto apply(::boost::tuples::null_type const&)
        { return hana::true_; }

        template <typename H, typename T>
        static constexpr auto apply(::boost::tuples::cons<H, T> const&)
        { return hana::false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct length_impl<ext::boost::Tuple> {
        template <typename Xs>
        static constexpr auto apply(Xs const&) {
            return hana::size_t< ::boost::tuples::length<Xs>::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, ext::boost::Tuple> {
        static constexpr bool value = true;
    };

    template <>
    struct make_impl<ext::boost::Tuple> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return ::boost::tuples::tuple<
                typename std::decay<Xs>::type...
            >{static_cast<Xs&&>(xs)...};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_HPP
