/*!
@file
Adapts `boost::fusion::list` for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/ext/boost/fusion/detail/common.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/tail.hpp>

#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/container/generation/make_list.hpp>
#include <boost/fusion/container/list.hpp>
#include <boost/fusion/container/list/convert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/version.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        struct list_tag;
    }}}

    template <typename T>
    struct tag_of<T, when<
        std::is_same<
            typename ::boost::fusion::traits::tag_of<T>::type,
            ::boost::fusion::traits::tag_of<
                ::boost::fusion::list<>
            >::type
        >::value
    >> {
        using type = ext::boost::fusion::list_tag;
    };

    namespace detail {
        template <>
        struct is_fusion_sequence<ext::boost::fusion::list_tag> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable (the rest is in detail/common.hpp)
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct tail_impl<ext::boost::fusion::list_tag> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::boost::fusion::as_list(
                ::boost::fusion::pop_front(static_cast<Xs&&>(xs)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::boost::fusion::list_tag> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return ::boost::fusion::make_list(static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_LIST_HPP
