/*!
@file
Defines the instance of `boost::hana::Applicative` for `boost::hana::Either`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_APPLICATIVE_HPP
#define BOOST_HANA_EITHER_APPLICATIVE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/either/either.hpp>

// Mcd
#include <boost/hana/either/functor.hpp>


namespace boost { namespace hana {
    //! Instance of `Applicative` for `Either`.
    //!
    //! The instance of `Applicative` for `Either` follows naturally from
    //! the instance of `Functor`. Specifically,
    //! @code
    //!     ap(left(x), anything) == left(x)
    //!     ap(right(x), left(anything)) == right(x)
    //!     ap(right(f), right(x)) == right(f(x))
    //!     lift<Either>(x) == right(x)
    //! @endcode
    //!
    //! ### Example
    //! @include example/either/applicative.cpp
    template <>
    struct Applicative::instance<Either> : Applicative::mcd {
        template <typename X>
        static constexpr decltype(auto) lift_impl(X&& x)
        { return right(detail::std::forward<X>(x)); }

        template <typename E, typename X>
        static constexpr decltype(auto) ap_impl(E&& e, X&& x) {
            return either(
                left,
                [x(detail::std::forward<X>(x))](auto&& f) -> decltype(auto) {
                    return fmap(x, detail::std::forward<decltype(f)>(f));
                },
                detail::std::forward<E>(e)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_APPLICATIVE_HPP
