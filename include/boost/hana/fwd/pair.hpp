/*!
@file
Forward declares `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PAIR_HPP
#define BOOST_HANA_FWD_PAIR_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/fwd/comparable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Generic container of two elements.
    //!
    //! ### Instance of
    //! `Comparable`, `Product`, `Foldable`
    struct Pair { struct hana_enabled_operators : Comparable { }; };

    namespace pair_detail {
        template <typename First, typename Second, typename = operators::enable_adl>
        struct pair {
            First first;
            Second second;
            using hana_datatype = Pair;
        };
    }

    //! Creates a `Pair` with the given elements.
    //! @relates Pair
    //!
    //! ### Example
    //! @snippet example/pair.cpp pair
    BOOST_HANA_CONSTEXPR_LAMBDA auto pair = [](auto first, auto second) {
        return pair_detail::pair<
            decltype(first), decltype(second)
        >{detail::std::move(first), detail::std::move(second)};
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PAIR_HPP
