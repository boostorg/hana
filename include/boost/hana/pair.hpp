/*!
@file
Defines `boost::hana::Pair`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_HPP
#define BOOST_HANA_PAIR_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/product/mcd.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Generic container of two elements.
    struct Pair { };

    namespace pair_detail {
        template <typename First, typename Second, typename = operators<Comparable>>
        struct pair {
            First first;
            Second second;
            using hana_datatype = Pair;
        };
    }

    //! Creates a `Pair` with the given elements.
    //! @relates Pair
    //!
    //! ### Example
    //! @snippet example/pair/pair.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto pair = [](auto first, auto second) {
        return pair_detail::pair<
            decltype(first), decltype(second)
        >{first, second};
    };

    //! Instance of `Product` for `Pair`s.
    //!
    //! ### Example
    //! @snippet example/pair/product.cpp main
    template <>
    struct Product::instance<Pair> : Product::mcd {
        template <typename X, typename Y>
        static constexpr auto make_product_impl(X x, Y y)
        { return pair(x, y); }

        template <typename P>
        static constexpr auto first_impl(P p)
        { return p.first; }

        template <typename P>
        static constexpr auto second_impl(P p)
        { return p.second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_HPP
