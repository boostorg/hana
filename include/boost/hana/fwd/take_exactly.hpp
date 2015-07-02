/*!
@file
Forward declares `boost::hana::take_exactly`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TAKE_EXACTLY_HPP
#define BOOST_HANA_FWD_TAKE_EXACTLY_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns the first `n` elements of a sequence, which must have at least
    //! `n` elements.
    //! @relates Sequence
    //!
    //! Given a `Sequence` `xs` and a `Constant` `n` holding an unsigned
    //! integral value, `take_exactly(xs, n)` is a new sequence containing
    //! the first `n` elements of `xs`, in the same order. It is an error if
    //! `n > length(xs)`. This guarantee allows `take_exactly` to be optimized
    //! better than the `take` function, which allows `n` to be greater than
    //! the number of elements in the sequence.
    //!
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //! @param n
    //! A non-negative `Constant` holding an unsigned integral value
    //! representing the number of elements to keep in the resulting sequence.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_exactly
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_exactly = [](auto&& xs, auto&& n) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_exactly_impl : take_exactly_impl<S, when<true>> { };

    struct take_exactly_t {
        template <typename Xs, typename N>
        constexpr auto operator()(Xs&& xs, N&& n) const;
    };

    constexpr take_exactly_t take_exactly{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TAKE_EXACTLY_HPP
