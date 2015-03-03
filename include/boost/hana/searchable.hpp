/*!
@file
Defines `boost::hana::Searchable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SEARCHABLE_HPP
#define BOOST_HANA_SEARCHABLE_HPP

#include <boost/hana/fwd/searchable.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // any_of
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct any_of_impl : any_of_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct any_of_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr void apply(Xs&&, Pred&&) {
            static_assert(wrong<any_of_impl<S>, Xs, Pred>{},
            "no definition of boost::hana::any_of for the given data type");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // any
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct any_impl : any_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct any_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::any_of(detail::std::forward<Xs>(xs), id); }
    };

    //////////////////////////////////////////////////////////////////////////
    // all_of
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct all_of_impl : all_of_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct all_of_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::not_(hana::any_of(detail::std::forward<Xs>(xs),
                    hana::compose(not_, detail::std::forward<Pred>(pred))));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // all
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct all_impl : all_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct all_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::all_of(detail::std::forward<Xs>(xs), id); }
    };

    //////////////////////////////////////////////////////////////////////////
    // none_of
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct none_of_impl : none_of_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct none_of_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::not_(hana::any_of(detail::std::forward<Xs>(xs),
                                        detail::std::forward<Pred>(pred)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // none
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct none_impl : none_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct none_impl<S, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::none_of(detail::std::forward<Xs>(xs), id); }
    };

    //////////////////////////////////////////////////////////////////////////
    // elem
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct elem_impl : elem_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct elem_impl<S, when<condition>> : default_ {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return hana::any_of(detail::std::forward<Xs>(xs),
                             hana::partial(equal, detail::std::forward<X>(x)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // find
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct find_impl : find_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct find_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr void apply(Xs&&, Pred&&) {
            static_assert(wrong<find_impl<S>, Xs, Pred>{},
            "no definition of boost::hana::find for the given data type");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // lookup
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct lookup_impl : lookup_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct lookup_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Key>
        static constexpr decltype(auto) apply(Xs&& xs, Key&& key) {
            return hana::find(detail::std::forward<Xs>(xs),
                    hana::partial(equal, detail::std::forward<Key>(key)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // subset
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct subset_impl : subset_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct subset_impl<S, when<condition>> : default_ {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ys) {
            return hana::all_of(detail::std::forward<Xs>(xs),
                    hana::partial(elem, detail::std::forward<Ys>(ys)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct models<Searchable(S)>
        : detail::std::integral_constant<bool,
            !is_default<any_of_impl<S>>{} &&
            !is_default<find_impl<S>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for builtin arrays
    //////////////////////////////////////////////////////////////////////////
    template <typename T, detail::std::size_t N>
    struct any_of_impl<T[N]> {
        template <typename Xs, typename Pred>
        static constexpr bool any_of_helper(bool cond, Xs&& xs, Pred&& pred) {
            if (cond) return true;
            for (detail::std::size_t i = 1; i < N; ++i)
                if (pred(detail::std::forward<Xs>(xs)[i]))
                    return true;
            return false;
        }

        // Since an array contains homogeneous data, if the predicate returns
        // a compile-time logical at any index, it must do so at every index
        // (because the type of the elements won't change)! In this case, we
        // then only need to evaluate the predicate on the first element.
        template <typename Xs, typename Pred>
        static constexpr auto
        any_of_helper(detail::std::true_type, Xs&& xs, Pred&&)
        { return true_; }

        template <typename Xs, typename Pred>
        static constexpr auto
        any_of_helper(detail::std::false_type, Xs&&, Pred&&)
        { return false_; }

        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            auto cond = hana::if_(pred(detail::std::forward<Xs>(xs)[0]),
                detail::std::true_type{},
                detail::std::false_type{}
            );
            return any_of_helper(cond, detail::std::forward<Xs>(xs),
                                       detail::std::forward<Pred>(pred));
        }
    };

    template <typename T, detail::std::size_t N>
    struct find_impl<T[N]> {
        using Size = detail::std::size_t;

        template <typename Xs, typename Pred, Size i>
        static constexpr decltype(auto)
        find_helper(Xs&& xs, Pred&& pred,
                    detail::std::integral_constant<Size, i>,
                    detail::std::false_type)
        {
            auto cond = pred(detail::std::forward<Xs>(xs)[i + 1]);
            constexpr bool truth_value = hana::if_(hana::value(cond), true, false);
            return find_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::integral_constant<Size, i + 1>{},
                detail::std::integral_constant<bool, truth_value>{}
            );
        }

        template <typename Xs, typename Pred, Size i>
        static constexpr decltype(auto)
        find_helper(Xs&& xs, Pred&&,
                    detail::std::integral_constant<Size, i>,
                    detail::std::true_type)
        {
            return hana::just(detail::std::forward<Xs>(xs)[i]);
        }

        template <typename Xs, typename Pred>
        static constexpr decltype(auto)
        find_helper(Xs&&, Pred&&,
                    detail::std::integral_constant<Size, N-1>,
                    detail::std::false_type)
        { return nothing; }

        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            auto cond = pred(detail::std::forward<Xs>(xs)[0]);
            constexpr bool truth_value = hana::if_(hana::value(cond), true, false);
            return find_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                detail::std::integral_constant<Size, 0>{},
                detail::std::integral_constant<bool, truth_value>{}
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SEARCHABLE_HPP
