/*!
@file
Defines `boost::hana::ext::boost::fusion::List`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/ext/boost/fusion/detail/common.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad_plus.hpp>

#include <boost/fusion/algorithm/transformation/join.hpp>
#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/container/list/convert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/version.hpp>

#include <type_traits>


#if BOOST_VERSION < 105800
#   error The adapter for fusion::list is not supported for versions of    \
          Boost prior to 1.58.0 because of bugs in fusion::list.
#endif

namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        struct List;
    }}}

    template <typename T>
    struct datatype<T, when<
        std::is_same<
            typename ::boost::fusion::traits::tag_of<T>::type,
            ::boost::fusion::traits::tag_of<
                ::boost::fusion::list<>
            >::type
        >::value
    >> {
        using type = ext::boost::fusion::List;
    };

    namespace detail {
        template <>
        struct is_fusion_sequence<ext::boost::fusion::List>
            : std::true_type
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable (the rest is in detail/common.hpp)
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct tail_impl<ext::boost::fusion::List> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::boost::fusion::as_list(
                ::boost::fusion::pop_front(static_cast<Xs&&>(xs)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct prepend_impl<ext::boost::fusion::List> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return ::boost::fusion::as_list(
                ::boost::fusion::push_front(
                    static_cast<Xs&&>(xs),
                    static_cast<X&&>(x)));
        }
    };

    template <>
    struct concat_impl<ext::boost::fusion::List> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return ::boost::fusion::as_list(
                ::boost::fusion::join(
                    static_cast<Xs&&>(xs),
                    static_cast<Ys&&>(ys)));
        }
    };

    template <>
    struct empty_impl<ext::boost::fusion::List> {
        static auto apply()
        { return ::boost::fusion::list<>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP
