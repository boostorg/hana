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
#include <boost/hana/detail/create.hpp>
#include <boost/hana/fwd/comparable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Generic container of two elements.
    //!
    //! ### Instance of
    //! `Comparable`, `Product`, `Foldable`
    struct Pair { struct hana { struct enabled_operators : Comparable { }; }; };

    //! Creates a `Pair` with the given elements.
    //! @relates Pair
    //!
    //! ### Example
    //! @snippet example/pair.cpp pair
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto pair = [](auto&& first, auto&& second) {
        return unspecified-type;
    };
#else
    template <typename First, typename Second, typename = operators::enable_adl>
    struct _pair {
        First first;
        Second second;
        struct hana { using datatype = Pair; };
    };

    constexpr detail::create<_pair> pair{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PAIR_HPP
