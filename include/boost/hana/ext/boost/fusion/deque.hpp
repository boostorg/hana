/*!
@file
Adapts `boost::fusion::deque` for use with Hana.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_DEQUE_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_DEQUE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/ext/boost/fusion/detail/common.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/hana/fwd/tail.hpp>

#include <boost/fusion/container/deque.hpp>
#include <boost/fusion/container/generation/make_deque.hpp>
#include <boost/fusion/support/tag_of.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        struct deque_tag;
    }}}

    template <typename T>
    struct tag_of<T, when<
        std::is_same<
            typename ::boost::fusion::traits::tag_of<T>::type,
            ::boost::fusion::traits::tag_of<
                ::boost::fusion::deque<>
            >::type
        >::value
    >> {
        using type = ext::boost::fusion::deque_tag;
    };

    namespace detail {
        template <>
        struct is_fusion_sequence<ext::boost::fusion::deque_tag> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable (the rest is in detail/common.hpp)
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct tail_impl<ext::boost::fusion::deque_tag> {
        template <typename Xs, std::size_t ...i>
        static constexpr auto tail_helper(Xs&& xs, std::index_sequence<0, i...>) {
            return ::boost::fusion::make_deque(::boost::fusion::at_c<i>(xs)...);
        }

        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            using Size = typename ::boost::fusion::result_of::size<Xs>::type;
            return tail_helper(static_cast<Xs&&>(xs),
                               std::make_index_sequence<Size::value>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::boost::fusion::deque_tag> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return ::boost::fusion::make_deque(static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_DEQUE_HPP
