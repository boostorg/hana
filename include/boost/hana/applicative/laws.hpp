/*!
@file
Defines `boost::hana::Applicative::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_LAWS_HPP
#define BOOST_HANA_APPLICATIVE_LAWS_HPP

#include <boost/hana/applicative/applicative.hpp>

#include <boost/hana/bool.hpp>


namespace boost { namespace hana {
    //! @details
    //! For any `Applicative` `A`, the following laws must be satisfied:
    //! @code
    //!     ap(lift<A>(id), u) == u                          // identity
    //!     ap(lift<A>(compose), u, v, w) == ap(u, ap(v, w)) // composition
    //!     ap(lift<A>(f), lift<A>(x)) == lift<A>(f(x))      // homomorphism
    //!     ap(u, lift<A>(y)) == ap(lift<A>(apply(y)), u)    // interchange
    //! @endcode
    //!
    //! As a consequence of these laws, the `Functor` instance for `A` will
    //! satisfy
    //! @code
    //!     fmap(f, x) == ap(lift<A>(f), x)
    //! @endcode
    //!
    //! @todo
    //! Actually implement and check the laws.
    struct Applicative::laws {
        template <typename A, typename U, typename V, typename W, typename X, typename Y, typename F>
        static constexpr auto check(A a, U u, V v, W w, X x, Y y, F f) {
            return true_;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_LAWS_HPP
