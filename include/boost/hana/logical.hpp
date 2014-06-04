/*!
@file
Defines the @ref Logical module.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_HPP

#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    //! @defgroup Logical Logical
    //! Logical operators and conditional branch selection.
    //!
    //! @todo
    //! - Use a non-naive implementation for `and_` and `or_`.
    //! - How to short-circuit?
    //! - Do we even need `and_` and `or_`? Perhaps we only need `&&` and `||`
    //! with the new paradigm?
    //! - Document the contents of this module.
    //!
    //! @{

    constexpr struct _and {
        template <typename X, typename ...Xs>
        constexpr auto operator()(X x, Xs ...xs) const
        { return x && (*this)(xs...); }

        constexpr auto operator()() const
        { return true_; }
    } and_{};

    constexpr struct _or {
        template <typename X, typename ...Xs>
        constexpr auto operator()(X x, Xs ...xs) const
        { return x || (*this)(xs...); }

        constexpr auto operator()() const
        { return false_; }
    } or_{};

    constexpr struct _if {
        template <typename Then, typename Else>
        constexpr auto operator()(decltype(true_), Then t, Else) const
        { return t; }

        template <typename Then, typename Else>
        constexpr auto operator()(decltype(false_), Then, Else e) const
        { return e; }

        template <typename Then, typename Else>
        constexpr auto operator()(bool cond, Then t, Else e) const
        { return cond ? t : e; }
    } if_{};

    //! @}
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP
