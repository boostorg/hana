/*!
@file
Defines `boost::hana::Comparable::product_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_PRODUCT_MCD_HPP
#define BOOST_HANA_COMPARABLE_PRODUCT_MCD_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/product/product.hpp>


namespace boost { namespace hana {
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_PRODUCT_MCD_HPP
