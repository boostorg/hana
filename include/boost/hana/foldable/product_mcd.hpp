/*!
@file
Defines `boost::hana::Foldable::product_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_PRODUCT_MCD_HPP
#define BOOST_HANA_FOLDABLE_PRODUCT_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/product/product.hpp>


namespace boost { namespace hana {
    struct Foldable::product_mcd : Foldable::unpack_mcd {
        template <typename P, typename F>
        static constexpr decltype(auto) unpack_impl(P&& p, F&& f) {
            return detail::std::forward<F>(f)(
                first(detail::std::forward<P>(p)),
                second(detail::std::forward<P>(p))
            );
        }
    };

    //! Minimal complete definition of `Foldable` for `Product`s.
    //!
    //! Folding a `Product` `p` is equivalent to folding a list containing
    //! `first(p)` and `second(p)`, in that order.
    template <typename P>
    struct Foldable::instance<P, when<is_a<Product, P>()>>
        : Foldable::product_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_PRODUCT_MCD_HPP
