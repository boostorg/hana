/*!
@file
Forward declares `boost::hana::at` and `boost::hana::at_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_AT_HPP
#define BOOST_HANA_FWD_AT_HPP

#include <boost/hana/core/when.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //! Returns the `n`th element of an iterable.
    //! @relates Iterable
    //!
    //! Given an `Iterable` and an `IntegralConstant` index, `at` returns the
    //! element located at the index in the linearization of the iterable.
    //! Specifically, given an iterable `xs` with a linearization of
    //! `[x1, ..., xN]`, `at(xs, k)` is equivalent to `xk`.
    //!
    //!
    //! @param xs
    //! The iterable in which an element is retrieved. The iterable must
    //! contain at least `n + 1` elements.
    //!
    //! @param n
    //! A non-negative `IntegralConstant` representing the 0-based index of
    //! the element to return. It is an error to call `at` with an index that
    //! out of bounds of the iterable.
    //!
    //!
    //! Example
    //! -------
    //! @include example/at.cpp
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.at.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto at = [](auto&& xs, auto const& n) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct at_impl : at_impl<It, when<true>> { };

    struct at_t {
        template <typename Xs, typename N>
        constexpr decltype(auto) operator()(Xs&& xs, N const& n) const;
    };

    constexpr at_t at{};
#endif

    //! Equivalent to `at`; provided for convenience.
    //! @relates Iterable
    //!
    //!
    //! Example
    //! -------
    //! @include example/at_c.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n, typename Xs>
    constexpr decltype(auto) at_c(Xs&& xs) {
        return hana::at(forwarded(xs), hana::size_c<n>);
    }
#else
    template <std::size_t n, typename Xs>
    constexpr decltype(auto) at_c(Xs&& xs);
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_AT_HPP
