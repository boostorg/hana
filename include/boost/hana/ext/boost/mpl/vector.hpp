/*!
@file
Defines `boost::hana::ext::boost::mpl::Vector`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP

#include <boost/hana/fwd/ext/boost/mpl/vector.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/boost/mpl/integral_c.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/vector/vector0.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct foldl_impl<ext::boost::mpl::Vector>
        : Iterable::foldl_impl<ext::boost::mpl::Vector>
    { };

    template <>
    struct foldr_impl<ext::boost::mpl::Vector>
        : Iterable::foldr_impl<ext::boost::mpl::Vector>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_impl<ext::boost::mpl::Vector>
        : Iterable::find_impl<ext::boost::mpl::Vector>
    { };

    template <>
    struct any_impl<ext::boost::mpl::Vector>
        : Iterable::any_impl<ext::boost::mpl::Vector>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<ext::boost::mpl::Vector> {
        template <typename xs>
        static constexpr auto apply(xs)
        { return type<typename ::boost::mpl::front<xs>::type>; }
    };

    template <>
    struct tail_impl<ext::boost::mpl::Vector> {
        template <typename xs>
        static constexpr auto apply(xs)
        { return typename ::boost::mpl::pop_front<xs>::type{}; }
    };

    template <>
    struct is_empty_impl<ext::boost::mpl::Vector> {
        template <typename xs>
        static constexpr auto apply(xs)
        { return typename ::boost::mpl::empty<xs>::type{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<ext::boost::mpl::Vector> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::foldr(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Ys>(ys),
                prepend
            );
        }
    };

    template <>
    struct prepend_impl<ext::boost::mpl::Vector> {
        template <typename X, typename Xs>
        static constexpr auto apply(X, Xs) {
            static_assert(detail::std::is_same<datatype_t<X>, Type>::value,
            "Only Types may be prepended to a Boost.MPL vector.");

            return typename ::boost::mpl::push_front<
                Xs, typename X::type
            >::type{};
        }
    };

    template <>
    struct nil_impl<ext::boost::mpl::Vector> {
        static constexpr auto apply()
        { return ::boost::mpl::vector0<>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
