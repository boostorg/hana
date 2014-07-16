/*!
@file
Adapts `boost::tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/iterable/foldable_instance.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/mcd.hpp>

#include <boost/tuple/tuple.hpp>


namespace boost { namespace hana {
    struct BoostTuple;

    template <typename ...Xs>
    struct datatype<boost::tuple<Xs...>> {
        using type = BoostTuple;
    };

    template <typename H, typename T>
    struct datatype<boost::tuples::cons<H, T>> {
        using type = BoostTuple;
    };

    template <>
    struct datatype<boost::tuples::null_type> {
        using type = BoostTuple;
    };

    template <>
    struct Iterable::instance<BoostTuple> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs tuple)
        { return tuple.get_head(); }

        template <typename Xs>
        static constexpr auto tail_impl(Xs tuple)
        { return tuple.get_tail(); }

        static constexpr auto is_empty_impl(boost::tuples::null_type const&)
        { return true_; }

        template <typename H, typename T>
        static constexpr auto is_empty_impl(boost::tuples::cons<H, T> const&)
        { return false_; }
    };

    template <>
    struct List::instance<BoostTuple> : List::mcd<BoostTuple> {
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return boost::tuples::cons<X, Xs>{x, xs}; }

        static constexpr auto nil_impl()
        { return boost::tuples::null_type{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_HPP
