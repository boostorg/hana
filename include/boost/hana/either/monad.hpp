/*!
@file
Defines the instance of `boost::hana::Monad` for `boost::hana::Either`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_MONAD_HPP
#define BOOST_HANA_EITHER_MONAD_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/either/either.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

// Mcd
#include <boost/hana/either/applicative.hpp>


namespace boost { namespace hana {
    //! Instance of `Monad` for `Either`.
    //!
    //! The instance of `Monad` for `Either` follows naturally from
    //! the instance of `Applicative`. Specifically,
    //! @code
    //!     flatten(right(right(x))) == right(x)
    //!     flatten(anything else) == anything else
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/either.cpp monad
    template <>
    struct Monad::instance<Either> : Monad::flatten_mcd<Either> {
        template <typename E>
        static constexpr decltype(auto) flatten_impl(E&& e) {
            return either(left, id, detail::std::forward<E>(e));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_MONAD_HPP
