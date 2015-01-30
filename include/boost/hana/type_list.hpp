/*!
@file
Defines `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_HPP
#define BOOST_HANA_TYPE_LIST_HPP

#include <boost/hana/fwd/type_list.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(TypeList)>
        : detail::std::true_type
    { };

    template <>
    struct equal_impl<TypeList, TypeList> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs, Ys)
        { return false_; }

        template <typename Xs>
        static constexpr auto apply(Xs, Xs)
        { return true_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(TypeList)>
        : detail::std::true_type
    { };

    template <>
    struct unpack_impl<TypeList> {
        //! @todo Fix the lost optimization caused by unpacking with `Type`s.
        template <typename ...Xs, typename F>
        static constexpr auto apply(_type_list<Xs...>, F&& f)
        { return detail::std::forward<F>(f)(type<Xs>...); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Iterable(TypeList)>
        : detail::std::true_type
    { };

    template <>
    struct head_impl<TypeList> {
        template <typename X, typename ...Xs>
        static constexpr auto apply(_type_list<X, Xs...>)
        { return type<X>; }
    };

    template <>
    struct tail_impl<TypeList> {
        template <typename X, typename ...Xs>
        static constexpr auto apply(_type_list<X, Xs...>)
        { return type_list<Xs...>; }
    };

    template <>
    struct is_empty_impl<TypeList> {
        template <typename ...Xs>
        static constexpr auto apply(_type_list<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_HPP
