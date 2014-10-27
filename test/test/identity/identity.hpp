/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_IDENTITY_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_IDENTITY_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace test {
    struct Identity { };

    template <typename T>
    struct identity_type {
        T value;
        struct hana { using datatype = Identity; };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto identity = [](auto x) {
        return identity_type<decltype(x)>{x};
    };
}

#endif //! BOOST_HANA_TEST_TEST_IDENTITY_IDENTITY_HPP
