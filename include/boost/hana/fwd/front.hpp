/*!
@file
Forward declares `boost::hana::front`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FRONT_HPP
#define BOOST_HANA_FWD_FRONT_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns the first element of a non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `front(xs)` is equal to `x1`. If `xs` is empty, it is an error to
    //! use this function.
    //!
    //!
    //! Example
    //! -------
    //! @include example/front.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto front = [](auto&& xs) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct front_impl : front_impl<It, when<true>> { };

    struct front_t {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const;
    };

    constexpr front_t front{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FRONT_HPP
