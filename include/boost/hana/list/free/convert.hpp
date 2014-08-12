/*!
@file
Defines the conversion from any `boost::hana::Foldable` to any `boost::hana::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LIST_FREE_CONVERT_HPP
#define BOOST_HANA_LIST_FREE_CONVERT_HPP

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/list/list.hpp>


namespace boost { namespace hana {
    //! Converts a `Foldable` to a `List`.
    template <typename L, typename T>
    struct convert<L, T, when<is_a<List, L>() && is_a<Foldable, T>()>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return foldr(xs, nil<L>, cons); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LIST_FREE_CONVERT_HPP
