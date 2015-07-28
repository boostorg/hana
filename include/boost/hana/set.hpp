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

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _set
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Xs>
    struct _set : operators::adl, detail::searchable_operators<_set<Xs...>> {
        _tuple<Xs...> storage;
        using hana = _set;
        using datatype = Set;

        explicit constexpr _set(_tuple<Xs...> const& xs)
            : storage(xs)
        { }

        explicit constexpr _set(_tuple<Xs...>&& xs)
            : storage(static_cast<_tuple<Xs...>&&>(xs))
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

            return _set<typename std::decay<Xs>::type...>{
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
        static constexpr decltype(auto) apply(S1&& s1, S2&& s2) {
            return hana::and_(
                hana::equal(hana::length(s1.storage), hana::length(s2.storage)),
                hana::is_subset(static_cast<S1&&>(s1), static_cast<S2&&>(s2))
            );
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
    struct to_impl<Set, F, when<_models<Foldable, F>{}()>> {
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
            template <typename S, typename X>
            constexpr decltype(auto) operator()(S&& s, X&& x) const {
                return hana::unpack(
                    hana::append(static_cast<S&&>(s).storage,
                                 static_cast<X&&>(x)),
                    hana::make<Set>
                );
            }
        };

        template <typename S, typename X>
        static constexpr decltype(auto) apply(S&& set, X&& x) {
            return hana::eval_if(hana::contains(set, x),
                hana::lazy(set),
                hana::lazy(insert_helper{})(set, x)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<Set> {
        template <typename S, typename X>
        static constexpr decltype(auto) apply(S&& set, X&& x) {
            return hana::unpack(
                hana::remove(static_cast<S&&>(set).storage,
                             static_cast<X&&>(x)),
                make<Set>
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP
