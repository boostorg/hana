/*!
@file
Defines the instance of `boost::hana::List` for `boost::hana::BoostTuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_TUPLE_LIST_HPP
#define BOOST_HANA_EXT_BOOST_TUPLE_LIST_HPP

#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/ext/boost/tuple/tuple.hpp>
#include <boost/hana/list/mcd.hpp>

// Mcd
#include <boost/hana/ext/boost/tuple/monad.hpp>
#include <boost/hana/ext/boost/tuple/foldable.hpp>
#include <boost/hana/ext/boost/tuple/iterable.hpp>

#include <boost/tuple/tuple.hpp>


namespace boost { namespace hana {
    template <>
    struct List::instance<BoostTuple> : List::mcd<BoostTuple> {
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return boost::tuples::cons<X, Xs>{
                detail::std::move(x),
                detail::std::move(xs)
            };
        }

        static constexpr auto nil_impl()
        { return boost::tuples::null_type{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_TUPLE_LIST_HPP
