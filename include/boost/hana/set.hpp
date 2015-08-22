/*!
@file
Defines `boost::hana::Set`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_HPP
#define BOOST_HANA_SET_HPP

#include <boost/hana/fwd/set.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/fast_and.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/searchable.hpp>
#include <boost/hana/erase_key.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/fwd/concept/constant.hpp>
#include <boost/hana/insert.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/tuple.hpp>

#include <boost/hana/difference.hpp>
#include <boost/hana/intersection.hpp>
#include <boost/hana/symmetric_difference.hpp>
#include <boost/hana/union.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // set
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Xs>
    struct set : operators::adl, detail::searchable_operators<set<Xs...>> {
        tuple<Xs...> storage;
        using hana = set;
        using datatype = Set;

        explicit constexpr set(tuple<Xs...> const& xs)
            : storage(xs)
        { }

        explicit constexpr set(tuple<Xs...>&& xs)
            : storage(static_cast<tuple<Xs...>&&>(xs))
        { }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<Set> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // make<Set>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Set> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(detail::fast_and<_models<Comparable, Xs>::value...>::value,
            "hana::make<Set>(xs...) requires all the 'xs' to be Comparable");

            static_assert(detail::fast_and<
                _models<Constant, decltype(hana::equal(xs, xs))>::value...
            >::value,
            "hana::make<Set>(xs...) requires all the 'xs' to be "
            "Comparable at compile-time");
        #endif

            return set<typename std::decay<Xs>::type...>{
                hana::make_tuple(static_cast<Xs&&>(xs)...)
            };
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Set, Set> {
        template <typename S1, typename S2>
        static constexpr auto equal_helper(S1 const& s1, S2 const& s2, decltype(hana::true_))
        { return hana::is_subset(s1, s2); }

        template <typename S1, typename S2>
        static constexpr auto equal_helper(S1 const&, S2 const&, decltype(hana::false_))
        { return hana::false_; }

        template <typename S1, typename S2>
        static constexpr decltype(auto) apply(S1&& s1, S2&& s2) {
            constexpr bool same_length = hana::value<decltype(
                hana::equal(hana::length(s1.storage), hana::length(s2.storage)
            ))>();
            return equal_helper(s1, s2, hana::bool_<same_length>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Set> {
        template <typename Set, typename F>
        static constexpr decltype(auto) apply(Set&& set, F&& f) {
            return hana::unpack(static_cast<Set&&>(set).storage,
                                static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<Set> {
        template <typename Set, typename Pred>
        static constexpr decltype(auto) apply(Set&& set, Pred&& pred) {
            return hana::find_if(static_cast<Set&&>(set).storage,
                              static_cast<Pred&&>(pred));
        }
    };

    template <>
    struct any_of_impl<Set> {
        template <typename Set, typename Pred>
        static constexpr decltype(auto) apply(Set&& set, Pred&& pred) {
            return hana::any_of(static_cast<Set&&>(set).storage,
                                static_cast<Pred&&>(pred));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct to_impl<Set, F, when<_models<Foldable, F>::value>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::fold_left(static_cast<Xs&&>(xs),
                                   hana::make<Set>(),
                                   hana::insert);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<Set> {
        struct insert_helper {
            template <typename Xs, typename X>
            constexpr decltype(auto) operator()(Xs&& xs, X&& x) const {
                return hana::unpack(
                    hana::append(static_cast<Xs&&>(xs).storage,
                                 static_cast<X&&>(x)),
                    hana::make_set
                );
            }
        };

        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return hana::eval_if(hana::contains(xs, x),
                hana::lazy(xs),
                hana::lazy(insert_helper{})(xs, x)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<Set> {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return hana::unpack(
                hana::remove(static_cast<Xs&&>(xs).storage,
                             static_cast<X&&>(x)),
                hana::make_set
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // intersection
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename Ys>
        struct set_insert_if_contains {
            Ys const& ys;

            template <typename Result, typename Key>
            static constexpr auto helper(Result&& result, Key&& key, decltype(hana::true_)) {
                return hana::insert(static_cast<Result&&>(result), static_cast<Key&&>(key));
            }

            template <typename Result, typename Key>
            static constexpr auto helper(Result&& result, Key&&, decltype(hana::false_)) {
                return static_cast<Result&&>(result);
            }

            template <typename Result, typename Key>
            constexpr auto operator()(Result&& result, Key&& key) const {
                constexpr bool keep = hana::value<decltype(hana::contains(ys, key))>();
                return set_insert_if_contains::helper(static_cast<Result&&>(result),
                                                      static_cast<Key&&>(key),
                                                      hana::bool_<keep>);
            }
        };
    }

    template <>
    struct intersection_impl<Set> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys const& ys) {
            return hana::fold_left(static_cast<Xs&&>(xs), hana::make_set(),
                                   detail::set_insert_if_contains<Ys>{ys});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // union_
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct union_impl<Set> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys&& ys) {
            return hana::fold_left(static_cast<Xs&&>(xs), static_cast<Ys&&>(ys),
                                   hana::insert);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // difference
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct difference_impl<Set> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs&& xs, Ys&& ys) {
            return hana::fold_left(static_cast<Ys&&>(ys), static_cast<Xs&&>(xs),
                                   hana::erase_key);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP
