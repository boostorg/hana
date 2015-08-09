/*!
@file
Forward declares `boost::hana::adjust_if`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ADJUST_IF_HPP
#define BOOST_HANA_FWD_ADJUST_IF_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Apply a function on all the elements of a structure satisfying a
    //! `predicate`.
    //! @relates Functor
    //!
    //!
    //! Signature
    //! ---------
    //! Given `F` a Functor and `Bool` a Logical, the signature is
    //! \f$
    //!     \mathtt{adjust_if} : F(T) \times (T \to Bool) \times (T \to T) \to F(T)
    //! \f$
    //!
    //! @param xs
    //! The structure to adjust with `f`.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for element(s) `x` of the
    //! structure and returning a `Logical` representing whether `f`
    //! should be applied to `x`.
    //!
    //! @param f
    //! A function called as `f(x)` on element(s) of the structure for which
    //! the `predicate` returns a true-valued `Logical`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/adjust_if.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto adjust_if = [](auto&& xs, auto&& predicate, auto&& f) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct adjust_if_impl : adjust_if_impl<Xs, when<true>> { };

    struct adjust_if_t {
        template <typename Xs, typename Pred, typename F>
        constexpr auto operator()(Xs&& xs, Pred&& pred, F&& f) const;
    };

    constexpr adjust_if_t adjust_if{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ADJUST_IF_HPP
