/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_EQUIVALENCE_CLASS_HPP
#define BOOST_HANA_TEST_TEST_EQUIVALENCE_CLASS_HPP

#include <boost/hana/concept/comparable.hpp>


namespace boost { namespace hana {
    namespace test {
        struct EquivalenceClass { };

        template <typename Token, typename T>
        struct equivalence_class_impl {
            Token equivalence_class;
            T unwrap;
            using hana_tag = EquivalenceClass;
        };

        auto tag = [](auto token, auto x) {
            return equivalence_class_impl<decltype(token), decltype(x)>{token, x};
        };
    }

    template <>
    struct equal_impl<test::EquivalenceClass, test::EquivalenceClass> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y)
        { return equal(x.equivalence_class, y.equivalence_class); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_EQUIVALENCE_CLASS_HPP
