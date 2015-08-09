/*!
@file
Forward declares `boost::hana::rem`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_REM_HPP
#define BOOST_HANA_FWD_REM_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Generalized integer remainder.
    //! @relates IntegralDomain
    //!
    //! Given two elements of an IntegralDomain `x` and `y`, with `y`
    //! nonzero, `rem` returns the remainder of the division of `x` by `y`.
    //! In other words, `rem` can be seen as an equivalent to `%`.
    //!
    //! Cross-type version of the method
    //! --------------------------------
    //! The `rem` method is "overloaded" to handle distinct data types
    //! with certain properties. Specifically, `rem` is defined for
    //! _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `IntegralDomain`s when taken individually
    //! 3. `to<C> : A -> B` and `to<C> : B -> C` are `Ring`-embeddings, as
    //!    determined by the `is_embedding` metafunction.
    //!
    //! In that case, `rem` is defined as
    //! @code
    //!     rem(x, y) = rem(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @include example/rem.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto rem = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct rem_impl : rem_impl<T, U, when<true>> { };

    struct rem_t {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const;
    };

    constexpr rem_t rem{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_REM_HPP
