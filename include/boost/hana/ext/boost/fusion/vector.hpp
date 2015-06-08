/*!
@file
Defines `boost::hana::ext::boost::fusion::Vector`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/boost/fusion/detail/common.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad_plus.hpp>

#include <boost/fusion/algorithm/transformation/join.hpp>
#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/support/tag_of.hpp>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        struct Vector;
    }}}

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename ::boost::fusion::traits::tag_of<T>::type,
            ::boost::fusion::traits::tag_of<
                ::boost::fusion::vector<>
            >::type
        >::value
    >> {
        using type = ext::boost::fusion::Vector;
    };

    namespace detail {
        template <>
        struct is_fusion_sequence<ext::boost::fusion::Vector>
            : detail::std::true_type
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable (the rest is in detail/common.hpp)
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct tail_impl<ext::boost::fusion::Vector> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::boost::fusion::as_vector(
                ::boost::fusion::pop_front(static_cast<Xs&&>(xs)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct prepend_impl<ext::boost::fusion::Vector> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return ::boost::fusion::as_vector(
                ::boost::fusion::push_front(
                    static_cast<Xs&&>(xs),
                    static_cast<X&&>(x)));
        }
    };

    template <>
    struct concat_impl<ext::boost::fusion::Vector> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return ::boost::fusion::as_vector(
                ::boost::fusion::join(
                    static_cast<Xs&&>(xs),
                    static_cast<Ys&&>(ys)));
        }
    };

    template <>
    struct empty_impl<ext::boost::fusion::Vector> {
        static auto apply()
        { return ::boost::fusion::vector<>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
