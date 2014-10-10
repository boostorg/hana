/*!
@file
Forward declares `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_SET_HPP
#define BOOST_HANA_SET_SET_HPP

#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! A basic unordered container requiring `Comparable` elements.
    struct Set { struct hana_enabled_operators : Comparable { }; };

    namespace set_detail {
        template <typename Storage, typename = operators::enable_adl>
        struct set {
            Storage storage;
            using hana_datatype = Set;
        };
    }

    //! Creates a `Set` containing the given elements.
    //! @relates Set
    //!
    //! @note
    //! There must not be duplicate elements.
    constexpr auto set = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto ...elements) {
        auto storage = tuple(detail::std::move(elements)...);
        return set_detail::set<decltype(storage)>{detail::std::move(storage)};
    };

#if 0
    // insert the given element in the set, or do nothing if it is already there
    BOOST_HANA_CONSTEXPR_LAMBDA auto insert = [](auto e, auto set) {
        return eval_if(elem(set, e),
            [=](auto) { return set; },
            [=](auto _) { return detail::wrap<Set>(_(cons)(e, detail::unwrap(set))); }
        );
    };

    template <>
    struct Foldable::instance<Set> : Foldable::mcd {
        template <typename Set, typename State, typename F>
        static constexpr auto foldr_impl(Set set, State s, F f) {
            return foldr(detail::unwrap(set), s, f);
        }

        template <typename Set, typename State, typename F>
        static constexpr auto foldl_impl(Set set, State s, F f) {
            return foldl(detail::unwrap(set), s, f);
        }

        template <typename Set>
        static constexpr auto length_impl(Set set)
        { return length(detail::unwrap(set)); }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_SET_HPP
