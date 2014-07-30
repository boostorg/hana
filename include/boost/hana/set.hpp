/*!
@file
Defines `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_HPP
#define BOOST_HANA_SET_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/instantiates.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/type_traits.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic unordered container requiring `Comparable` elements.
    struct Set { };

    //! Creates a `Set` containing the given elements.
    //! @relates Set
    //!
    //! @note
    //! There must not be duplicate elements.
    BOOST_HANA_CONSTEXPR_LAMBDA auto set = [](auto ...elements) {
        return detail::wrap<Set>(list(elements...));
    };

    //! Two sets are equal iff they contain the same elements, regardless of
    //! the order.
    //!
    //! ### Example
    //! example/set/comparable.cpp main
    template <>
    struct Comparable::instance<Set, Set> : Comparable::equal_mcd {
        template <typename S1, typename S2>
        static constexpr auto equal_impl(S1 s1, S2 s2) {
            return and_(
                equal(length(detail::unwrap(s1)), length(detail::unwrap(s2))),
                all([=](auto k) {
                    return equal(lookup(k, s1), lookup(k, s2));
                }, detail::unwrap(s1))
            );
        }
    };

    //! The keys and the values of a `Set` are its elements; the `Searchable` 
    //! instance follows naturally from that.
    //!
    //! ### Example
    //! example/set/searchable.cpp main
    template <>
    struct Searchable::instance<Set> : Searchable::mcd {
        template <typename Pred, typename Set>
        static constexpr auto find_impl(Pred pred, Set set)
        { return find(pred, detail::unwrap(set)); }

        template <typename Pred, typename Set>
        static constexpr auto any_impl(Pred pred, Set set)
        { return any(pred, detail::unwrap(set)); }
    };

    //! Converts a `List` to a `Set`.
    //! @relates Set
    //!
    //! @note
    //! The list must not contain duplicate keys.
    template <typename L>
    struct convert<Set, L, detail::std::enable_if_t<instantiates<List, L>()>> {
        template <typename Xs>
        static constexpr auto apply(Xs xs)
        { return unpack(set, xs); }
    };

    //! Converts a `Set` to a `List`.
    //! @relates Set
    template <typename L>
    struct convert<L, Set, detail::std::enable_if_t<instantiates<List, L>()>> {
        template <typename Set>
        static constexpr auto apply(Set set)
        { return to<L>(detail::unwrap(set)); }
    };

#if 0
    // insert the given element in the set, or do nothing if it is already there
    BOOST_HANA_CONSTEXPR_LAMBDA auto insert = [](auto e, auto set) {
        return eval_if(elem(e, set),
            [=](auto) { return set; },
            [=](auto _) { return detail::wrap<Set>(_(cons)(e, detail::unwrap(set))); }
        );
    };

    template <>
    struct Foldable::instance<Set> : Foldable::mcd {
        template <typename F, typename State, typename Set>
        static constexpr auto foldr_impl(F f, State s, Set set) {
            return foldr(f, s, detail::unwrap(set));
        }

        template <typename F, typename State, typename Set>
        static constexpr auto foldl_impl(F f, State s, Set set) {
            return foldl(f, s, detail::unwrap(set));
        }

        template <typename Set>
        static constexpr auto length_impl(Set set)
        { return length(detail::unwrap(set)); }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP
