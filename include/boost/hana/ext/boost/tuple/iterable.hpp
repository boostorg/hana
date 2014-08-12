/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::BoostTuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_ITERABLE_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/ext/boost/tuple/tuple.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <boost/tuple/tuple.hpp>


namespace boost { namespace hana {
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_ITERABLE_HPP
