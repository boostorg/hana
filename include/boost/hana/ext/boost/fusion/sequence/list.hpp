/*!
@file
Defines the instance of `boost::hana::List` for `boost::hana::BoostFusionSequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_LIST_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_LIST_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/ext/boost/fusion/sequence/sequence.hpp>
#include <boost/hana/list/mcd.hpp>

// Mcd
#include <boost/hana/ext/boost/fusion/sequence/monad.hpp>
#include <boost/hana/ext/boost/fusion/sequence/foldable.hpp>
#include <boost/hana/ext/boost/fusion/sequence/iterable.hpp>

#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/container/list/cons.hpp>


namespace boost { namespace hana {
    template <>
    struct List::instance<BoostFusionSequence>
        : List::mcd<BoostFusionSequence>
    {
        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            return boost::fusion::push_front(
                    detail::std::forward<Xs>(xs),
                    detail::std::forward<X>(x));
        }

        static auto nil_impl()
        { return boost::fusion::nil{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_SEQUENCE_LIST_HPP
