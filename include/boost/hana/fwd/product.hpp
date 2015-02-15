/*!
@file
Forward declares `boost::hana::Product`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PRODUCT_HPP
#define BOOST_HANA_FWD_PRODUCT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! Represents types that are generic containers of two elements.
    //!
    //! Specifically, the `Product` concept represents types that are
    //! [category theoretical products][1].
    //!
    //!
    //! Laws
    //! ----
    //! For a model `P` of `Product`, the following laws must be satisfied,
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
    //! property, all the models of this concept are isomorphic.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `first`, `second` and `make`
    //! `first` and `second` must obviously return the first and the second
    //! element of the pair, respectively. `make` must take two arguments `x`
    //! and `y` representing the first and the second element of the pair,
    //! and return a pair `p` such that `first(p) == x` and `second(p) == y`.
    //! Example:
    //! @snippet example/product.cpp make
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. `Comparable`\n
    //! Two products `x` and `y` are equal iff they are equal element-wise,
    //! i.e. iff
    //! @code
    //!     first(x) == first(y) && second(x) == second(y)
    //! @endcode
    //! Example:
    //! @snippet example/product.cpp comparable
    //!
    //! 2. `Orderable`\n
    //! Products are ordered using a lexicographical ordering as-if they
    //! were 2-element tuples.
    //!
    //! 3. `Foldable`\n
    //! Folding a `Product` `p` is equivalent to folding a list containing
    //! `first(p)` and `second(p)`, in that order.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Product_(category_theory)
    struct Product { };

    //! Returns the first element of a product.
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
        template <typename P>
        constexpr decltype(auto) operator()(P&& p) const {
            return first_impl<typename datatype<P>::type>::apply(
                detail::std::forward<P>(p)
            );
        }
    };

    constexpr _first first{};
#endif

    //! Returns the second element of a product.
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
        template <typename P>
        constexpr decltype(auto) operator()(P&& p) const {
            return second_impl<typename datatype<P>::type>::apply(
                detail::std::forward<P>(p)
            );
        }
    };

    constexpr _second second{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PRODUCT_HPP
