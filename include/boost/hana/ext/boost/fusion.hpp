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
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/iteration/reverse_fold.hpp>
#include <boost/fusion/algorithm/query/all.hpp>
#include <boost/fusion/algorithm/query/any.hpp>
#include <boost/fusion/algorithm/query/count_if.hpp>
#include <boost/fusion/algorithm/query/none.hpp>
#include <boost/fusion/algorithm/transformation/join.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>
#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/algorithm/transformation/push_back.hpp>
#include <boost/fusion/algorithm/transformation/push_front.hpp>
#include <boost/fusion/algorithm/transformation/replace_if.hpp>
#include <boost/fusion/algorithm/transformation/reverse.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
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

        template <typename T>
        struct is_bidirectional_sequence {
            static constexpr bool value = std::is_base_of<
                boost::fusion::bidirectional_traversal_tag,
                typename boost::fusion::traits::category_of<T>::type
            >::value;
        };

        template <typename T>
        struct is_random_access_sequence {
            static constexpr bool value = std::is_base_of<
                boost::fusion::random_access_traversal_tag,
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
    struct Foldable::instance<FusionNonAssociativeForwardSequence>
        : Iterable::FoldableInstance
    {
        template <typename F, typename State, typename Xs>
        static constexpr decltype(auto) foldl_impl(F f, State s, Xs&& xs) {
            return boost::fusion::fold(std::forward<Xs>(xs), s, f);
        }

        template <typename F, typename State, typename Xs, typename =
            detail::enable_if_t<
                fusion_detail::is_bidirectional_sequence<
                    std::remove_reference_t<Xs>
                >::value
            >
        >
        static constexpr decltype(auto) foldr_impl(F f, State s, Xs&& xs) {
            auto g = [=](auto state, auto x) { return f(x, state); };
            return boost::fusion::reverse_fold(std::forward<Xs>(xs), s, g);
        }

        template <typename F, typename State, typename Xs, typename =
            detail::enable_if_t<
                !fusion_detail::is_bidirectional_sequence<
                    std::remove_reference_t<Xs>
                >::value
            >
        >
        static constexpr decltype(auto) foldr_impl(F f, State s, Xs&& xs, ...) {
            return Iterable::FoldableInstance::foldr_impl(f, s, std::forward<Xs>(xs));
        }

        template <typename Xs>
        static constexpr auto length_impl(Xs&& xs) {
            using Size = decltype(boost::fusion::size(std::forward<Xs>(xs)));
            return size_t<Size::value>;
        }

        template <typename Pred, typename Xs>
        static constexpr auto any_impl(Pred p, Xs&& xs) {
            return boost::fusion::any(std::forward<Xs>(xs), p);
        }

        template <typename Pred, typename Xs>
        static constexpr auto all_impl(Pred p, Xs&& xs) {
            return boost::fusion::all(std::forward<Xs>(xs), p);
        }

        template <typename Pred, typename Xs>
        static constexpr auto none_impl(Pred p, Xs&& xs) {
            return boost::fusion::none(std::forward<Xs>(xs), p);
        }

        template <typename Pred, typename Xs>
        static constexpr auto count_impl(Pred p, Xs&& xs) {
            return boost::fusion::count_if(std::forward<Xs>(xs), p);
        }
    };

    template <>
    struct Iterable::instance<FusionNonAssociativeForwardSequence>
        : Iterable::mcd
    {
        template <typename Xs>
        static constexpr auto is_empty_impl(Xs&& xs) {
            using R = decltype(boost::fusion::empty(std::forward<Xs>(xs)));
            return bool_<R::value>;
        }

        template <typename Xs>
        static constexpr decltype(auto) head_impl(Xs&& xs) {
            return boost::fusion::front(std::forward<Xs>(xs));
        }

        template <typename Xs>
        static constexpr decltype(auto) tail_impl(Xs&& xs) {
            return boost::fusion::pop_front(std::forward<Xs>(xs));
        }

        template <typename Xs, typename = detail::enable_if_t<
            fusion_detail::is_bidirectional_sequence<
                std::remove_reference_t<Xs>
            >::value
        >>
        static constexpr decltype(auto) last_impl(Xs&& xs) {
            return boost::fusion::back(std::forward<Xs>(xs));
        }

        template <typename Xs, typename = detail::enable_if_t<
            !fusion_detail::is_bidirectional_sequence<
                std::remove_reference_t<Xs>
            >::value
        >>
        static constexpr decltype(auto) last_impl(Xs&& xs, ...) {
            return Iterable::mcd::last_impl(std::forward<Xs>(xs));
        }

        template <typename N, typename Xs, typename = detail::enable_if_t<
            fusion_detail::is_random_access_sequence<
                std::remove_reference_t<Xs>
            >::value
        >>
        static constexpr decltype(auto) at_impl(N n, Xs&& xs) {
            return boost::fusion::at_c<n()>(std::forward<Xs>(xs));
        }

        template <typename N, typename Xs, typename = detail::enable_if_t<
            !fusion_detail::is_random_access_sequence<
                std::remove_reference_t<Xs>
            >::value
        >>
        static constexpr decltype(auto) at_impl(N n, Xs&& xs, ...) {
            return Iterable::mcd::at_impl(n, std::forward<Xs>(xs));
        }

        template <typename F, typename Xs>
        static constexpr decltype(auto) for_each_impl(Xs&& xs, F f) {
            return boost::fusion::for_each(std::forward<Xs>(xs), f);
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

        template <typename Xs, typename = detail::enable_if_t<
            fusion_detail::is_bidirectional_sequence<
                std::remove_reference_t<Xs>
            >::value
        >>
        static constexpr decltype(auto) reverse_impl(Xs&& xs) {
            return boost::fusion::reverse(std::forward<Xs>(xs));
        }

        template <typename Xs, typename = detail::enable_if_t<
            !fusion_detail::is_bidirectional_sequence<
                std::remove_reference_t<Xs>
            >::value
        >>
        static constexpr decltype(auto) reverse_impl(Xs&& xs, ...) {
            return List::mcd<FusionNonAssociativeForwardSequence>::
                   reverse_impl(std::forward<Xs>(xs));
        }

        template <typename Xs>
        static constexpr decltype(auto) init_impl(Xs&& xs) {
            return boost::fusion::pop_back(std::forward<Xs>(xs));
        }

        template <typename Xs, typename Ys>
        static constexpr decltype(auto) concat_impl(Xs&& xs, Ys&& ys) {
            return boost::fusion::join(std::forward<Xs>(xs), std::forward<Ys>(ys));
        }

        template <typename Xs, typename X>
        static constexpr decltype(auto) snoc_impl(Xs&& xs, X x) {
            return boost::fusion::push_back(std::forward<Xs>(xs), x);
        }
    };

    template <>
    struct Functor::instance<FusionNonAssociativeForwardSequence>
        : Functor::fmap_mcd
    {
        template <typename F, typename Xs>
        static constexpr decltype(auto) fmap_impl(F f, Xs&& xs) {
            return boost::fusion::transform(std::forward<Xs>(xs), f);
        }

        template <typename Pred, typename X, typename Xs>
        static constexpr decltype(auto) replace_impl(Pred p, X x, Xs&& xs) {
            return boost::fusion::replace_if(std::forward<Xs>(xs), p, x);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
