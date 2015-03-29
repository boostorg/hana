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

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/insert_fwd.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _set
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Xs>
    struct _set : operators::adl {
        _tuple<Xs...> storage;
        using hana = _set;
        using datatype = Set;

        _set() = default;
        _set(_set const&) = default;
        _set(_set&&) = default;
        _set(_set&) = default;
        template <typename ...Ys>
        explicit constexpr _set(Ys&& ...ys)
            : storage{static_cast<Ys&&>(ys)...}
        { }
    };

    //////////////////////////////////////////////////////////////////////////
    // make<Set>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Set> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return _set<typename detail::std::decay<Xs>::type...>{
                static_cast<Xs&&>(xs)...
            };
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Set>
        : operators::of<Comparable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Set, Set> {
        template <typename S1, typename S2>
        static constexpr decltype(auto) apply(S1&& s1, S2&& s2) {
            return hana::and_(
                hana::equal(hana::length(s1.storage), hana::length(s2.storage)),
                hana::subset(static_cast<S1&&>(s1), static_cast<S2&&>(s2))
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
    struct to_impl<Set, F, when<_models<Foldable, F>{}>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::foldr(static_cast<Xs&&>(xs),
                                set(), hana::flip(insert));
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
                    hana::prepend(
                        static_cast<X&&>(x),
                        static_cast<S&&>(s).storage
                    ),
                    hana::set
                );
            }
        };

        template <typename S, typename X>
        static constexpr decltype(auto) apply(S&& set, X&& x) {
            return hana::eval_if(hana::elem(set, x),
                hana::lazy(set),
                hana::lazy(insert_helper{})(set, x)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP
