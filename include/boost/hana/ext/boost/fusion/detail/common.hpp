/*!
@file
Defines common methods for all Boost.Fusion sequences.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_DETAIL_COMMON_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_DETAIL_COMMON_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/sequence.hpp>

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>


namespace boost { namespace hana {
    namespace detail {
        template <typename T>
        struct is_fusion_sequence
            : detail::std::false_type
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct head_impl<S, when<detail::is_fusion_sequence<S>{}>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return ::boost::fusion::front(detail::std::forward<Xs>(xs)); }
    };

    template <typename S>
    struct is_empty_impl<S, when<detail::is_fusion_sequence<S>{}>> {
        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            using Empty = decltype(
                ::boost::fusion::empty(detail::std::forward<Xs>(xs))
            );
            return bool_<Empty::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct models<Sequence(S), when<detail::is_fusion_sequence<S>{}>>
        : detail::std::true_type
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_DETAIL_COMMON_HPP
