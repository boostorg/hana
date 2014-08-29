/*!
@file
Defines `boost::hana::Traversable::traverse_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_TRAVERSE_MCD_HPP
#define BOOST_HANA_TRAVERSABLE_TRAVERSE_MCD_HPP

#include <boost/hana/traversable/traversable.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `traverse`
    struct Traversable::traverse_mcd {
        template <typename A, typename T>
        static constexpr auto sequence_impl(T traversable)
        { return traverse<A>(traversable, [](auto x) { return x; }); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_TRAVERSE_MCD_HPP
