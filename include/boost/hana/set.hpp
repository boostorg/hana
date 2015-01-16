/*!
@file
Defines `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_HPP
#define BOOST_HANA_SET_HPP

#include <boost/hana/fwd/set.hpp>

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/insert_fwd.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/logical.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //! Two sets are equal iff they contain the same elements, regardless of
    //! the order.
    //!
    //! ### Example
    //! @snippet example/set.cpp comparable
    template <>
    struct equal_impl<Set, Set> {
        template <typename S1, typename S2>
        static constexpr decltype(auto) apply(S1&& s1, S2&& s2) {
            return and_(
                equal(length(s1.storage), length(s2.storage)),
                subset(detail::std::forward<S1>(s1), detail::std::forward<S2>(s2))
            );
        }
    };

    //! Converts a `Foldable` to a `Set`.
    //! @relates Set
    //!
    //! If the foldable structure contains duplicates, the last one will
    //! be the one appearing in the resulting set.
    template <typename F>
    struct convert<Set, F, when<is_a<Foldable, F>()>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return foldr(detail::std::forward<Xs>(xs), set(), flip(insert));
        }
    };

    //! Converts a `Set` to a `List`.
    //! @relates Set
    //!
    //! The order of the elements in the resulting list is unspecified.
    template <typename L>
    struct convert<L, Set, when<is_a<List, L>()>> {
        template <typename Set>
        static constexpr decltype(auto) apply(Set&& set)
        { return to<L>(detail::std::forward<Set>(set).storage); }
    };

    template <>
    struct insert_impl<Set> {
        template <typename S, typename X>
        static constexpr decltype(auto) apply(S&& set, X&& x) {
            return eval_if(elem(set, x),
                [&set](auto) -> decltype(auto) {
                    return id(detail::std::forward<S>(set));
                },
                [&set, &x](auto _) -> decltype(auto) {
                    return unpack(
                        _(cons)(
                            detail::std::forward<X>(x),
                            detail::std::forward<S>(set).storage
                        ),
                        hana::set
                    );
                }
            );
        }
    };

    //! The keys and the values of a `Set` are its elements; the `Searchable` 
    //! instance follows naturally from that.
    //!
    //! ### Example
    //! @snippet example/set.cpp searchable
    template <>
    struct find_impl<Set> {
        template <typename Set, typename Pred>
        static constexpr decltype(auto) apply(Set&& set, Pred&& pred) {
            return find(
                detail::std::forward<Set>(set).storage,
                detail::std::forward<Pred>(pred)
            );
        }
    };

    template <>
    struct any_impl<Set> {
        template <typename Set, typename Pred>
        static constexpr decltype(auto) apply(Set&& set, Pred&& pred) {
            return any(
                detail::std::forward<Set>(set).storage,
                detail::std::forward<Pred>(pred)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP
