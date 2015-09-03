/*!
@file
Forward declares `boost::hana::tail`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TAIL_HPP
#define BOOST_HANA_FWD_TAIL_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns a new iterable containing all but the first element of a
    //! non-empty iterable.
    //! @relates Iterable
    //!
    //! Given a non-empty Iterable `xs` with a linearization of `[x1, ..., xN]`,
    //! `tail(xs)` is an Iterable with the same tag whose linearization is
    //! `[x2, ..., xN]`. In particular, `tail(xs)` is functionally equivalent
    //! to `drop(xs, size_t<1>)`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/tail.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto tail = [](auto&& xs) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct tail_impl : tail_impl<It, when<true>> { };

    struct tail_t {
        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr tail_t tail{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TAIL_HPP
