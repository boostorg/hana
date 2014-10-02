/*!
@file
Defines the instance of `boost::hana::Monoid` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_MONOID_HPP
#define BOOST_HANA_FOREIGN_MONOID_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/monoid/mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Monoid` for foreign objects with numeric types.
    //!
    //! Any two foreign objects that can be added with the usual `operator+`
    //! and for which a valid conversion from `int` exists (for both)
    //! naturally form an additive `Monoid`, with `0` being the identity
    //! and the usual `operator+` being the associative operation.
    template <typename T, typename U>
    struct Monoid::instance<T, U, when<is_valid<decltype((void)(
        static_cast<T>(0),
        static_cast<U>(0),
        *(T*)0 + *(U*)0
    ))>>> : Monoid::mcd {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) + detail::std::forward<Y>(y);
        }

        // Will never be used with two different `T` and `U` anyway.
        static constexpr auto zero_impl()
        { return static_cast<T>(0); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_MONOID_HPP
