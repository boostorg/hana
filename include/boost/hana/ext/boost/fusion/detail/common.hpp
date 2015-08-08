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
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/value.hpp>

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>

#include <cstddef>


namespace boost { namespace hana {
    namespace detail {
        template <typename T>
        struct is_fusion_sequence {
            static constexpr bool value = false;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct at_impl<S, when<detail::is_fusion_sequence<S>::value>> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t n = hana::value<N>();
            return ::boost::fusion::at_c<n>(static_cast<Xs&&>(xs));
        }
    };

    template <typename S>
    struct is_empty_impl<S, when<detail::is_fusion_sequence<S>::value>> {
        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            using Empty = decltype(::boost::fusion::empty(xs));
            return hana::bool_<Empty::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct length_impl<S, when<detail::is_fusion_sequence<S>::value>> {
        template <typename Xs>
        static constexpr auto apply(Xs const&) {
            using Size = typename ::boost::fusion::result_of::size<Xs>::type;
            return hana::size_t<Size::value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct models_impl<Sequence, S, when<detail::is_fusion_sequence<S>::value>> {
        static constexpr bool value = true;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_DETAIL_COMMON_HPP