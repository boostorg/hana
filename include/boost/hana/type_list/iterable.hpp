/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_ITERABLE_HPP
#define BOOST_HANA_TYPE_LIST_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list/type_list.hpp>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<TypeList> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs)
        { return head_impl(typename Xs::storage{}); }

        template <typename X, typename ...Xs>
        static constexpr auto head_impl(detail::repr::type_list<X, Xs...>)
        { return type<X>; }


        template <typename Xs>
        static constexpr auto tail_impl(Xs)
        { return tail_impl(typename Xs::storage{}); }

        template <typename X, typename ...Xs>
        static constexpr auto tail_impl(detail::repr::type_list<X, Xs...>)
        { return type_list<Xs...>; }


        template <typename Xs>
        static constexpr auto is_empty_impl(Xs)
        { return is_empty_impl(typename Xs::storage{}); }

        template <typename ...Xs>
        static constexpr auto is_empty_impl(detail::repr::type_list<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_ITERABLE_HPP
