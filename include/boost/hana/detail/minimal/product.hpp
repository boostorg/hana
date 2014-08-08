/*!
@file
Defines minimal instances for the `boost::hana::Product` type class.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_PRODUCT_HPP
#define BOOST_HANA_DETAIL_MINIMAL_PRODUCT_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/product/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename X, typename Y, typename mcd,
        typename = operators<hana::Comparable>>
    struct product_type {
        X fst;
        Y snd;
        using hana_datatype = mcd;
    };

    template <typename mcd>
    struct make_product_impl {
        template <typename X, typename Y>
        constexpr auto operator()(X x, Y y) const
        { return product_type<X, Y, mcd>{x, y}; }
    };

    template <typename mcd = hana::Product::mcd>
    constexpr make_product_impl<mcd> product{};
}} // end namespace detail::minimal

template <>
struct Product::instance<Product::mcd>
    : Product::mcd
{
    template <typename X, typename Y>
    static constexpr auto make_product_impl(X x, Y y)
    { return detail::minimal::product<Product::mcd>(x, y); }

    template <typename P>
    static constexpr auto first_impl(P p)
    { return p.fst; }

    template <typename P>
    static constexpr auto second_impl(P p)
    { return p.snd; }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_PRODUCT_HPP
