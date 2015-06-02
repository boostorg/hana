/*!
@file
Forward declares `boost::hana::Product`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PRODUCT_HPP
#define BOOST_HANA_FWD_PRODUCT_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! Represents types that are generic containers of two elements.
    //!
    //! This concept basically represents types that are like `std::pair`.
    //! The motivation for making such a precise concept is similar to the
    //! motivation behind the `Sequence` concept; there are many different
    //! implementations of `std::pair` in different libraries, and we would
    //! like to manipulate any of them generically.
    //!
    //! Since a `Product` is basically a pair, it is unsurprising that the
    //! operations provided by this concept are getting the first and second
    //! element of a pair, creating a pair from two elements and other
    //! simmilar operations.
    //!
    //! @note
    //! Mathematically, this concept represents types that are category
    //! theoretical [products][1]. This is also where the name comes
    //! from.
    //!
    //!
    //! Laws
    //! ----
    //! For a model `P` of `Product`, the following laws must be satisfied.
    //! For every data types `X` and `Y`, there must be a unique function
    //! @f$ make : X \times Y \to P @f$ such that for every `x`, `y`,
    //! @code
    //!     x == first(make<P>(x, y))
    //!     y == second(make<P>(x, y))
    //! @endcode
    //!
    //! @note
    //! This law is less general than the universal property typically used to
    //! define category theoretical products, but it is vastly enough for what
    //! we need.
    //!
    //! This is basically saying that a `Product` must be the most general
    //! object able to contain a pair of objects `(P1, P2)`, but nothing
    //! more. Since the categorical product is defined by a universal
    //! property, all the models of this concept are isomorphic, and
    //! the isomorphism is unique. In other words, there is one and only
    //! one way to convert one `Product` to another.
    //!
    //! Another property that must be satisfied by `first` and `second` is
    //! that of @ref move-independence, which ensures that we can optimally
    //! decompose a `Product` into its two members without making redundant
    //! copies.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! __Definitions of `first`, `second` and `make` satisfying the laws__\n
    //! `first` and `second` must obviously return the first and the second
    //! element of the pair, respectively. `make` must take two arguments `x`
    //! and `y` representing the first and the second element of the pair,
    //! and return a pair `p` such that `first(p) == x` and `second(p) == y`.
    //! @snippet example/product.cpp make
    //!
    //!
    //! Superclasses
    //! ------------
    //! 1. `Comparable` (free model)\n
    //! Two products `x` and `y` are equal iff they are equal element-wise,
    //! by comparing the first element before the second element.
    //! @snippet example/product.cpp comparable
    //!
    //! 2. `Orderable` (free model)\n
    //! Products are ordered using a lexicographical ordering as-if they
    //! were 2-element tuples.
    //!
    //! 3. `Foldable` (free model)\n
    //! Folding a `Product` `p` is equivalent to folding a list containing
    //! `first(p)` and `second(p)`, in that order.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Pair`
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Product_(category_theory)
    struct Product { };

    //! Returns the first element of a pair.
    //! @relates Product
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/product.cpp first
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto first = [](auto&& product) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename P, typename = void>
    struct first_impl;

    struct _first {
        template <typename Pair>
        constexpr decltype(auto) operator()(Pair&& pair) const {
            using P = typename datatype<Pair>::type;
            using First = first_impl<P>;

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Product, P>{},
            "hana::first(pair) requires pair to be a Product");
        #endif

            return First::apply(static_cast<Pair&&>(pair));
        }
    };

    constexpr _first first{};
#endif

    //! Returns the second element of a pair.
    //! @relates Product
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/product.cpp second
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto second = [](auto&& product) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename P, typename = void>
    struct second_impl;

    struct _second {
        template <typename Pair>
        constexpr decltype(auto) operator()(Pair&& pair) const {
            using P = typename datatype<Pair>::type;
            using Second = second_impl<P>;

        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(_models<Product, P>{},
            "hana::second(pair) requires pair to be a Product");
        #endif

            return Second::apply(static_cast<Pair&&>(pair));
        }
    };

    constexpr _second second{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PRODUCT_HPP
