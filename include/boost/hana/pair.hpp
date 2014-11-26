/*!
@file
Defines `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/fwd/pair.hpp>

#include <boost/hana/detail/std/forward.hpp>

// instances
#include <boost/hana/product.hpp>


namespace boost { namespace hana {
    //! Instance of `Product` for `Pair`s.
    //!
    //! ### Example
    //! @snippet example/pair.cpp product
    template <>
    struct Product::instance<Pair> : Product::mcd {
        template <typename F, typename S>
        static constexpr decltype(auto) make_impl(F&& f, S&& s)
        { return pair(detail::std::forward<F>(f), detail::std::forward<S>(s)); }

        template <typename P>
        static constexpr decltype(auto) first_impl(P&& p)
        { return detail::std::forward<P>(p).first; }

        template <typename P>
        static constexpr decltype(auto) second_impl(P&& p)
        { return detail::std::forward<P>(p).second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
