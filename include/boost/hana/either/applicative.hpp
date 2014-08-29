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
        static constexpr auto lift_impl(X x)
        { return right(x); }

        template <typename E, typename _>
        static constexpr auto ap_impl(either_detail::left<E> e, _)
        { return e; }

        template <typename F, typename X>
        static constexpr auto ap_impl(either_detail::right<F> f, X x)
        { return fmap(x, f.value); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_APPLICATIVE_HPP
