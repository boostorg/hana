/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::BoostFusionSequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_ITERABLE_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/ext/boost/fusion/sequence/sequence.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<BoostFusionSequence>
        : Iterable::mcd
    {
        template <typename Xs>
        static constexpr auto is_empty_impl(Xs&& xs) {
            using R = decltype(boost::fusion::empty(detail::std::forward<Xs>(xs)));
            return bool_<R::value>;
        }

        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return boost::fusion::front(detail::std::forward<Xs>(xs));
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            return boost::fusion::pop_front(detail::std::forward<Xs>(xs));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_ITERABLE_HPP
