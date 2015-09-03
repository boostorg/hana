/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP

#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/concept/product.hpp>


namespace boost { namespace hana {
    namespace test {
        struct MinimalProduct;

        template <typename X, typename Y>
        struct product_t {
            X fst;
            Y snd;
            using hana_tag = MinimalProduct;
        };

        constexpr detail::create<product_t> minimal_product{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<test::MinimalProduct> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return test::minimal_product(x, y); }
    };

    template <>
    struct first_impl<test::MinimalProduct> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return p.fst; }
    };

    template <>
    struct second_impl<test::MinimalProduct> {
        template <typename P>
        static constexpr decltype(auto) apply(P&& p)
        { return p.snd; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP
