/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_EQUIVALENCE_CLASS_HPP
#define BOOST_HANA_TEST_TEST_EQUIVALENCE_CLASS_HPP

namespace test {
    struct EquivalenceClass { };

    template <typename Token, typename T>
    struct equivalence_class_impl {
        Token equivalence_class;
        T unwrap;
        using hana_datatype = EquivalenceClass;
    };

    auto tag = [](auto token, auto x) {
        return equivalence_class_impl<decltype(token), decltype(x)>{token, x};
    };
} // end namespace test


#include <boost/hana/comparable/equal_mcd.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<test::EquivalenceClass, test::EquivalenceClass>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return equal(x.equivalence_class, y.equivalence_class);
        }
    };
}}

#endif // !BOOST_HANA_TEST_TEST_EQUIVALENCE_CLASS_HPP
