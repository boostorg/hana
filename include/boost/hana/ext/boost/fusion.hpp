/*!
@file
Adapts all non-associative Boost.Fusion sequences.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>

#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    namespace fusion_detail {
        template <typename T, bool = boost::fusion::traits::is_sequence<T>::value>
        struct is_nonassociative_forward_sequence {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_nonassociative_forward_sequence<T, true> {
            static constexpr bool value = !std::is_base_of<
                boost::fusion::associative_tag,
                typename boost::fusion::traits::category_of<T>::type
            >::value;
        };
    }

    struct FusionNonAssociativeForwardSequence;

    template <typename T>
    struct datatype<T, detail::enable_if_t<
        fusion_detail::is_nonassociative_forward_sequence<T>::value
    >> {
        using type = FusionNonAssociativeForwardSequence;
    };

    template <>
    struct Iterable::instance<FusionNonAssociativeForwardSequence>
        : Iterable::mcd
    {
        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs) {
            using IsEmpty = decltype(boost::fusion::empty(xs));
            return bool_<IsEmpty::value>;
        }

        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return boost::fusion::front(std::forward<Xs>(xs));
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            return boost::fusion::pop_front(std::forward<Xs>(xs));
        }
    };

    template <>
    struct List::instance<FusionNonAssociativeForwardSequence>
        : List::mcd<FusionNonAssociativeForwardSequence>
    {
        template <typename X, typename Xs>
        static constexpr decltype(auto) cons_impl(X&& x, Xs&& xs) {
            return boost::fusion::push_front(
                std::forward<Xs>(xs), std::forward<X>(x));
        }

        static auto nil_impl()
        { return boost::fusion::nil{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
