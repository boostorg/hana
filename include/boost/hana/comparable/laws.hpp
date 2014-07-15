/*!
@file
Defines `boost::hana::Comparable::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_LAWS_HPP
#define BOOST_HANA_COMPARABLE_LAWS_HPP

#include <boost/hana/comparable/comparable.hpp>

#include <boost/hana/detail/logical_fwd.hpp>


namespace boost { namespace hana {
    //! @details
    //! `equal` must define an equivalence relation. In other words, for all
    //! `a`, `b`, `c` of comparable data types,
    //! @code
    //!     a == a                          // Reflexivity
    //!     if a == b then b == a           // Symmetry
    //!     if a == b && b == c then a == c // Transitivity
    //! @endcode
    struct Comparable::laws {
        template <typename A, typename B, typename C>
        static constexpr auto check(A a, B b, C c) {
            auto implies = [](auto p, auto q) { return or_(not_(p), q); };
            return and_(
                equal(a, a),
                implies(equal(a, b), equal(b, a)),
                implies(and_(equal(a, b), equal(b, c)), equal(a, c))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_LAWS_HPP

#include <boost/hana/logical.hpp>
