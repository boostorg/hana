/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_FOLDABLE_HPP
#define BOOST_HANA_MAYBE_FOLDABLE_HPP

#include <boost/hana/foldable/folds_mcd.hpp>
#include <boost/hana/maybe/maybe.hpp>


namespace boost { namespace hana {
    //! Instance of `Foldable` for `Maybe`s.
    //!
    //! Folding a `Maybe` is equivalent to folding a `List` containing either
    //! no elements (for `nothing`) or `x` (for `just(x)`). The `Foldable`
    //! instance follows from that.
    //!
    //! ### Example
    //! @snippet example/maybe/foldable.cpp main
    template <>
    struct Foldable::instance<Maybe>
        : Foldable::folds_mcd
    {
        template <typename M, typename S, typename F>
        static constexpr auto foldr_impl(M m, S s, F f)
        { return maybe(s, [=](auto x) { return f(x, s); }, m); }

        template <typename M, typename S, typename F>
        static constexpr auto foldl_impl(M m, S s, F f)
        { return maybe(s, [=](auto x) { return f(s, x); }, m); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_FOLDABLE_HPP
