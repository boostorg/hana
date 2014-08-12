/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP

#include <boost/hana/product/mcd.hpp>

// Provided instances
#include <boost/hana/comparable/product_mcd.hpp>


namespace test {
    template <typename X, typename Y, typename Mcd>
    struct product_type {
        X fst;
        Y snd;
        using hana_datatype = Mcd;
    };

    template <typename Mcd>
    struct make_product_impl {
        template <typename X, typename Y>
        constexpr auto operator()(X x, Y y) const
        { return product_type<X, Y, Mcd>{x, y}; }
    };

    template <typename Mcd = boost::hana::Product::mcd>
    constexpr make_product_impl<Mcd> minimal_product{};
}

namespace boost { namespace hana {
    template <>
    struct Product::instance<Product::mcd>
        : Product::mcd
    {
        template <typename X, typename Y>
        static constexpr auto make_product_impl(X x, Y y)
        { return test::minimal_product<Product::mcd>(x, y); }

        template <typename P>
        static constexpr auto first_impl(P p)
        { return p.fst; }

        template <typename P>
        static constexpr auto second_impl(P p)
        { return p.snd; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP
