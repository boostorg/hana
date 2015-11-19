/*!
@file
Forward declares `boost::hana::back`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_BACK_HPP
#define BOOST_HANA_FWD_BACK_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/core/when.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! Returns the last element of a non-empty and finite iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty and finite iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `back(xs)` is equal to `xN`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/back.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto back = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct back_impl : back_impl<It, when<true>> { };

    struct back_t {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const;
    };

    constexpr back_t back{};
#endif
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_FWD_BACK_HPP
