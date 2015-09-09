/*!
@file
Adapts `boost::tuple` for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/fwd/at.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/is_empty.hpp>
#include <boost/hana/fwd/length.hpp>
#include <boost/hana/fwd/tail.hpp>
#include <boost/hana/integral_constant.hpp>

#include <boost/tuple/tuple.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace boost { struct tuple_tag; }}

    template <typename ...Xs>
    struct tag_of<boost::tuple<Xs...>> {
        using type = ext::boost::tuple_tag;
    };

    template <typename H, typename T>
    struct tag_of<boost::tuples::cons<H, T>> {
        using type = ext::boost::tuple_tag;
    };

    template <>
    struct tag_of<boost::tuples::null_type> {
        using type = ext::boost::tuple_tag;
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<ext::boost::tuple_tag> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t n = N::value;
            return static_cast<Xs&&>(xs).template get<n>();
        }
    };

    template <>
    struct tail_impl<ext::boost::tuple_tag> {
        template <typename H>
        static constexpr auto
        apply(boost::tuples::cons<H, boost::tuples::null_type> const&)
        { return boost::tuples::null_type{}; }

        template <typename H>
        static constexpr auto
        apply(boost::tuples::cons<H, boost::tuples::null_type>&)
        { return boost::tuples::null_type{}; }

        template <typename H, typename T>
        static constexpr auto const& apply(boost::tuples::cons<H, T> const& xs)
        { return xs.get_tail(); }

        template <typename H, typename T>
        static constexpr auto& apply(boost::tuples::cons<H, T>& xs)
        { return xs.get_tail(); }

        template <typename H, typename T>
        static constexpr auto apply(boost::tuples::cons<H, T>&& xs)
        { return std::move(xs).get_tail(); }
    };

    template <>
    struct is_empty_impl<ext::boost::tuple_tag> {
        static constexpr auto apply(boost::tuples::null_type const&)
        { return hana::true_c; }

        template <typename H, typename T>
        static constexpr auto apply(boost::tuples::cons<H, T> const&)
        { return hana::false_c; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct length_impl<ext::boost::tuple_tag> {
        template <typename Xs>
        static constexpr auto apply(Xs const&) {
            return hana::size_c<boost::tuples::length<Xs>::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct Sequence<ext::boost::tuple_tag> {
        static constexpr bool value = true;
    };

    template <>
    struct make_impl<ext::boost::tuple_tag> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return boost::tuples::tuple<
                typename std::decay<Xs>::type...
            >{static_cast<Xs&&>(xs)...};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_HPP
