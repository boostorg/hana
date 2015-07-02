/*!
@file
Forward declares `boost::hana::take_until`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TAKE_UNTIL_HPP
#define BOOST_HANA_FWD_TAKE_UNTIL_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Take elements from a sequence until the `predicate` is satisfied.
    //! @relates Sequence
    //!
    //! Specifically, `take_until` returns a new sequence containing the
    //! longest prefix of `xs` in which all elements do not satisfy the
    //! predicate. This is effectively equivalent to `take_while` with a
    //! negated predicate.
    //!
    //!
    //! @param xs
    //! The sequence to take the elements from.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! sequence, and returning a `Logical` representing whether the resulting
    //! sequence should stop at the element before `x`. In the current version
    //! of the library, `predicate` has to return a `Constant Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/sequence.cpp take_until
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto take_until = [](auto&& xs, auto&& predicate) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct take_until_impl : take_until_impl<S, when<true>> { };

    struct take_until_t {
        template <typename Xs, typename Pred>
        constexpr auto operator()(Xs&& xs, Pred&& pred) const;
    };

    constexpr take_until_t take_until{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TAKE_UNTIL_HPP
