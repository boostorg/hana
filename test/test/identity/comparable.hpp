/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_COMPARABLE_HPP

#include <boost/hana/comparable.hpp>
#include <test/identity/identity.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<test::Identity, test::Identity>
        : Comparable::equal_mcd
    {
        template <typename Id1, typename Id2>
        static constexpr auto equal_impl(Id1 x, Id2 y) {
            return equal(x.value, y.value);
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_IDENTITY_COMPARABLE_HPP
