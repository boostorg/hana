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

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical.hpp>

// provided instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    template <typename T, typename U>
    struct equal_impl<T, U, when<is_a<Product, T>() && is_a<Product, U>()>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return and_(
                equal(first(x), first(y)),
                equal(second(x), second(y))
            );
        }
    };

    template <typename T, typename U>
    struct less_impl<T, U, when<is_a<Product, T>() && is_a<Product, U>()>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return or_(
                less(first(x), first(y)),
                and_(
                    less_equal(first(x), first(y)),
                    less(second(x), second(y))
                )
            );
        }
    };

    template <typename Prod>
    struct unpack_impl<Prod, when<is_a<Product, Prod>()>> {
        template <typename P, typename F>
        static constexpr decltype(auto) apply(P&& p, F&& f) {
            return detail::std::forward<F>(f)(
                first(detail::std::forward<P>(p)),
                second(detail::std::forward<P>(p))
            );
        }
    };

    template <>
    struct models_impl<Product> {
        template <typename P, typename Context>
        static constexpr auto apply =
            is_implemented<first_impl<P>, Context> &&
            is_implemented<second_impl<P>, Context> &&
            is_implemented<make_impl<P>, Context>
        ;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRODUCT_HPP
