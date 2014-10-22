/*!
@file
Defines `boost::hana::Product`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PRODUCT_HPP
#define BOOST_HANA_PRODUCT_HPP

#include <boost/hana/fwd/product.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical.hpp>

// provided instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `first`, `second` and `make`.
    struct Product::mcd { };

    //! Mininal complete definition: `Product`
    struct Comparable::product_mcd : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) equal_impl(X const& x, Y const& y) {
            return and_(
                equal(first(x), first(y)),
                equal(second(x), second(y))
            );
        }
    };

    //! Instance of `Comparable` for `Product`s.
    //!
    //! Two products `x` and `y` are equal iff they are equal element-wise,
    //! i.e. iff
    //! @code
    //!     first(x) == first(y) && second(x) == second(y)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/product.cpp comparable
    template <typename T, typename U>
    struct Comparable::instance<T, U, when<
        is_a<Product, T>() && is_a<Product, U>()
    >> : Comparable::product_mcd { };

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

#endif // !BOOST_HANA_PRODUCT_HPP
