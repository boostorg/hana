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
    //!
    //! @todo
    //! Use perfect forwarding in `first_impl` and `second_impl` once
    //! bug http://llvm.org/bugs/show_bug.cgi?id=20619 is fixed.
    template <>
    struct Product::instance<Pair> : Product::mcd {
        template <typename F, typename S>
        static constexpr decltype(auto) make_impl(F&& f, S&& s)
        { return pair(detail::std::forward<F>(f), detail::std::forward<S>(s)); }

        template <typename P>
        static constexpr auto first_impl(P p)
        { return p.first; }

        template <typename P>
        static constexpr auto second_impl(P p)
        { return p.second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
