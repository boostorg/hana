/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_NUMERIC_NUMERIC_HPP
#define BOOST_HANA_TEST_TEST_NUMERIC_NUMERIC_HPP

#include <boost/hana/core/datatype.hpp>


namespace test {
    struct numeric_type {
        int value;
        //! @todo Find a way to make this explicit.
        constexpr operator bool() const { return value; }
    };

    using Numeric = boost::hana::datatype_t<numeric_type>;

    struct _numeric {
        constexpr numeric_type operator()(int x) const {
            return {x};
        }
    };
    constexpr _numeric numeric{};
}

#endif //! BOOST_HANA_TEST_TEST_NUMERIC_NUMERIC_HPP
