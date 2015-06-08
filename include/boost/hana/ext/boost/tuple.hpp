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
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/sequence.hpp>

#include <boost/tuple/tuple.hpp>


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
    struct head_impl<ext::boost::Tuple> {
        template <typename Xs>
        static constexpr auto const& apply(Xs const& tuple)
        { return tuple.get_head(); }

        template <typename Xs>
        static constexpr auto& apply(Xs& tuple)
        { return tuple.get_head(); }

        template <typename Xs>
        static constexpr auto apply(Xs&& tuple)
        { return detail::std::move(tuple.get_head()); }
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
        { return detail::std::move(xs.get_tail()); }
    };

    template <>
    struct is_empty_impl<ext::boost::Tuple> {
        static constexpr auto apply(::boost::tuples::null_type const&)
        { return true_; }

        template <typename H, typename T>
        static constexpr auto apply(::boost::tuples::cons<H, T> const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct prepend_impl<ext::boost::Tuple> {
        template <typename Xs, typename X>
        static constexpr auto apply(Xs xs, X x) {
            return ::boost::tuples::cons<X, Xs>{
                detail::std::move(x),
                detail::std::move(xs)
            };
        }
    };

    template <>
    struct empty_impl<ext::boost::Tuple> {
        static constexpr auto apply()
        { return ::boost::tuples::null_type{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, ext::boost::Tuple>
        : decltype(true_)
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_HPP
