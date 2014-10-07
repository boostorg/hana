/*!
@file
Defines the instance of `boost::hana::Ring` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_RING_HPP
#define BOOST_HANA_FOREIGN_RING_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/ring/mcd.hpp>

// Mcd
#include <boost/hana/foreign/group.hpp>


namespace boost { namespace hana {
    //! Instance of `Ring` for foreign objects with numeric types.
    //!
    //! Any two foreign objects that are `Group`s, that can be multiplied
    //! with the usual `operator*` and for which a valid conversion from `int`
    //! exists (for both) naturally form a multiplicative `Ring`, with `1`
    //! being the identity and the usual `operator*` being the ring operation.
    template <typename T, typename U>
    struct Ring::instance<T, U, when<
        are<Group, T, U>() &&
        is_valid<decltype((void)(
            static_cast<T>(1),
            static_cast<U>(1),
            *(T*)1 * *(U*)1
        ))>
    >> : Ring::mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) mult_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) * detail::std::forward<Y>(y);
        }

        // Will never be used with two different `T` and `U` anyway.
        static constexpr auto one_impl()
        { return static_cast<T>(1); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_RING_HPP
