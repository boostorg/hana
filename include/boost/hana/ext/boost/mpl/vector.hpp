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
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/boost/mpl/integral_c.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/vector/vector0.hpp>


namespace boost { namespace hana {
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
    struct prepend_impl<ext::boost::mpl::Vector> {
        template <typename X, typename Xs>
        static constexpr auto apply(X, Xs) {
            using T = typename datatype<X>::type;
            static_assert(detail::std::is_same<T, Type>{},
            "trying to prepend a non-Type to a Boost.MPL vector");

            return typename ::boost::mpl::push_front<
                Xs, typename X::type
            >::type{};
        }
    };

    template <>
    struct empty_impl<ext::boost::mpl::Vector> {
        static constexpr auto apply()
        { return ::boost::mpl::vector0<>{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Sequence(ext::boost::mpl::Vector)>
        : detail::std::true_type
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_HPP
