/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_NUMERIC_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_NUMERIC_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace test {
    struct numeric_type {
        int value;
        //! @todo Find a way to make this explicit.
        constexpr operator bool() const { return value; }
    };

    using Numeric = boost::hana::datatype_t<numeric_type>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto numeric = [](auto x) {
        return numeric_type{x};
    };
}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_NUMERIC_HPP
