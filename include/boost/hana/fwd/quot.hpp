/*!
@file
Forward declares `boost::hana::quot`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_QUOT_HPP
#define BOOST_HANA_FWD_QUOT_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Generalized integer quotient.
    //! @relates IntegralDomain
    //!
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `quot` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `quot` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `IntegralDomain`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Ring`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! In that case, the `quot` method is defined as
    //! @code
    //!     quot(x, y) = quot(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @include example/quot.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto quot = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct quot_impl : quot_impl<T, U, when<true>> { };

    struct quot_t {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const;
    };

    constexpr quot_t quot{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_QUOT_HPP
