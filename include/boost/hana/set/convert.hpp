/*!
@file
Defines conversions to and from a `boost::hana::Map`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_CONVERT_HPP
#define BOOST_HANA_SET_CONVERT_HPP

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/set/set.hpp>


namespace boost { namespace hana {
    //! Converts a `Foldable` to a `Set`.
    //! @relates Set
    //!
    //! @note
    //! The list must not contain duplicate keys.
    //!
    //! @todo
    //! Allow duplicate keys.
    template <typename F>
    struct convert<Set, F, when<is_a<Foldable, F>()>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return unpack(detail::std::forward<Xs>(xs), set); }
    };

    //! Converts a `Set` to a `List`.
    //! @relates Set
    template <typename L>
    struct convert<L, Set, when<is_a<List, L>()>> {
        template <typename Set>
        static constexpr decltype(auto) apply(Set&& set)
        { return to<L>(detail::std::forward<Set>(set).storage); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_CONVERT_HPP
