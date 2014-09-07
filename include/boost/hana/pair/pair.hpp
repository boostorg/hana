/*!
@file
Forward declares `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_FWD_HPP
#define BOOST_HANA_PAIR_FWD_HPP

#include <boost/hana/comparable/operators.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Generic container of two elements.
    //!
    //! ### Instance of
    //! `Comparable`, `Product`
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
    constexpr auto pair = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto first, auto second) {
        return pair_detail::pair<
            decltype(first), decltype(second)
        >{first, second};
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_FWD_HPP
