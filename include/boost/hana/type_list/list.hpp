/*!
@file
Defines the instance of `boost::hana::List` for `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_LIST_HPP
#define BOOST_HANA_TYPE_LIST_LIST_HPP

#include <boost/hana/list/mcd.hpp>
#include <boost/hana/type_list/type_list.hpp>

// Mcd
#include <boost/hana/type_list/monad.hpp>
#include <boost/hana/type_list/foldable.hpp>
#include <boost/hana/type_list/iterable.hpp>


namespace boost { namespace hana {
    template <>
    struct List::instance<TypeList> : List::mcd<TypeList> {
        template <typename X, typename ...Xs>
        static constexpr auto cons_impl(X, detail::repr::type_list<Xs...>)
        { return type_list<typename X::type, Xs...>; }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return cons_impl(x, typename Xs::storage{}); }

        static constexpr auto nil_impl()
        { return type_list<>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_LIST_HPP
