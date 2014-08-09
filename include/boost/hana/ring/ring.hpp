/*!
@file
Forward declares `boost::hana::Ring`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RING_RING_HPP
#define BOOST_HANA_RING_RING_HPP

#include <boost/hana/ring/detail/ring_fwd.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/group/detail/group_fwd.hpp>


namespace boost { namespace hana {
    //! Instance of `Ring` for objects with numeric data types.
    //!
    //! Any two objects whose data types are `Group`s, can be multiplied with
    //! the usual `operator*` and for which a valid conversion from `int`
    //! exists (for both data types) naturally form a multiplicative `Ring`,
    //! with `1` being the identity and the usual `operator*` being the ring
    //! operation.
    template <typename X, typename Y>
    struct Ring::instance<X, Y, when<
        are<Group, X, Y>() &&
        is_valid<decltype((void)(
            static_cast<X>(1),
            static_cast<Y>(1),
            *(X*)1 * *(Y*)1
        ))>
    >> : Ring::mcd {
        static constexpr auto mult_impl(X x, Y y)
        { return x * y; }

        // Will never be used with two different `X` and `Y` anyway.
        static constexpr auto one_impl()
        { return static_cast<X>(1); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_RING_HPP
