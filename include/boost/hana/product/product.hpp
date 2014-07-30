/*!
@file
Forward declares `boost::hana::Product`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PRODUCT_PRODUCT_HPP
#define BOOST_HANA_PRODUCT_PRODUCT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Represents types that are generic containers of two elements.
    //!
    //! Specifically, the `Product` type class represents types that are
    //! [category theoretical products][Wikipedia.Product].
    //!
    //! ### Laws
    //! For an instance `P` of `Product`, the following laws must be satisfied,
    //! which is equivalent to satisfying the universal property exposed in
    //! the article linked above. For every data types `X`, `P1`, `P2` and
    //! functions `f : X -> P1`, `g : X -> P2`, there exists a unique function
    //! `make : X -> P` such that for all object `x` of data type `X`,
    //! @code
    //!     f(x) == first(make(x))
    //!     g(x) == second(make(x))
    //! @endcode
    //!
    //! This is basically saying that a `Product` must be the most general
    //! object able to contain a pair of objects `(P1, P2)`, but nothing
    //! more. Since the categorical product is defined by a universal
    //! property, all instances are unique up to isomorphism.
    //!
    //!
    //! [Wikipedia.Product]: http://en.wikipedia.org/wiki/Product_(category_theory)
    struct Product {
        BOOST_HANA_TYPECLASS(Product);
        struct mcd;
    };

    //! Creates a `Product` of the two given elements.
    //! @relates Product
    //!
    //!
    //! @tparam P
    //! The data type (an instance of `Product`) of the product to create.
    //!
    //! @param fst
    //! The first element of the resulting product.
    //!
    //! @param snd
    //! The second element of the resulting product.
    //!
    //!
    //! ### Example
    //! @snippet example/product/make_product.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename P>
    constexpr auto make_product = [](auto fst, auto snd) {
        return Product::instance<
            datatype_t<P>
        >::make_product_impl(fst, snd);
    };
#else
    namespace product_detail {
        template <typename P>
        struct make_product {
            template <typename Fst, typename Snd>
            constexpr auto operator()(Fst fst, Snd snd) const {
                return Product::instance<
                    datatype_t<P>
                >::make_product_impl(fst, snd);
            }
        };
    }

    template <typename P>
    constexpr product_detail::make_product<P> make_product{};
#endif

    //! Return the first element of a product.
    //! @relates Product
    //!
    //! ### Example
    //! @snippet example/product/first.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto first = [](auto product) {
        return Product::instance<
            datatype_t<decltype(product)>
        >::first_impl(product);
    };

    //! Return the second element of a product.
    //! @relates Product
    //!
    //! ### Example
    //! @snippet example/product/second.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto second = [](auto product) {
        return Product::instance<
            datatype_t<decltype(product)>
        >::second_impl(product);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRODUCT_PRODUCT_HPP
