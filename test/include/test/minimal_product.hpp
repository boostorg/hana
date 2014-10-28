/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP

#include <boost/hana/detail/create.hpp>
#include <boost/hana/product.hpp>


namespace boost { namespace hana {
    namespace test {
        struct MinimalProduct;

        template <typename X, typename Y>
        struct _product {
            X fst;
            Y snd;
            struct hana { using datatype = MinimalProduct; };
        };

        constexpr detail::create<_product> minimal_product{};
    }

    template <>
    struct Product::instance<test::MinimalProduct>
        : Product::mcd
    {
        template <typename X, typename Y>
        static constexpr auto make_impl(X x, Y y)
        { return test::minimal_product(x, y); }

        template <typename P>
        static constexpr auto first_impl(P p)
        { return p.fst; }

        template <typename P>
        static constexpr auto second_impl(P p)
        { return p.snd; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_PRODUCT_HPP
