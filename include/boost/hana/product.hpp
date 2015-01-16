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

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical.hpp>

// provided instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>


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

    template <typename P>
    constexpr auto is_a<Product, P> = bool_<
        is_implemented<first_impl<P>> &&
        is_implemented<second_impl<P>> &&
        is_implemented<make_impl<P>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_PRODUCT_HPP
