/*!
 * @file
 * Defines `boost::hana::Foldable`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_HPP

namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Foldable;

    constexpr struct _foldl {
        template <typename F, typename State, typename Foldable_>
        constexpr auto operator()(F f, State s, Foldable_ foldable) const
        { return Foldable<Foldable_>::foldl_impl(f, s, foldable); }
    } foldl{};

    constexpr struct _foldr {
        template <typename F, typename State, typename Foldable_>
        constexpr auto operator()(F f, State s, Foldable_ foldable) const
        { return Foldable<Foldable_>::foldr_impl(f, s, foldable); }
    } foldr{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
