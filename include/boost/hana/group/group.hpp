/*!
@file
Forward declares `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_GROUP_HPP
#define BOOST_HANA_GROUP_GROUP_HPP

#include <boost/hana/group/detail/group_fwd.hpp>

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/monoid/monoid.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `minus`
    template <typename G1, typename G2>
    struct Group::minus_mcd {
        template <typename X>
        static constexpr auto negate_impl(X x) {
            return minus(zero<G1>, x);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_GROUP_HPP

#ifndef BOOST_HANA_GROUP_GROUP_MORE_HPP
#define BOOST_HANA_GROUP_GROUP_MORE_HPP

#include <boost/hana/core/is_a.hpp>


namespace boost { namespace hana {
    //! Instance of `Group` for objects with numeric data types.
    //!
    //! Any two objects whose data types form a `Monoid` and can be subtracted
    //! with the usual `operator-` naturally form an additive group, with
    //! the group subtraction being that usual `operator-`.
    template <typename X, typename Y>
    struct Group::instance<X, Y, when<
        are<Monoid, X, Y>()
        &&
        (
            decltype((void)(*(X*)0 - *(Y*)0), 1){}, true

        )
    >>
        : Group::minus_mcd<X, Y>
    {
        static constexpr auto minus_impl(X x, Y y) {
            return x - y;
        }
    };
}}

#endif
