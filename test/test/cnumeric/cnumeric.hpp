/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_CNUMERIC_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_CNUMERIC_HPP

namespace test {
    struct CNumeric { };

    template <typename T, T v>
    struct cnumeric_type {
        static constexpr T value = v;
        using hana_datatype = CNumeric;

        //! @todo Find a way to make this explicit.
        constexpr operator bool() const { return value; }
    };

    template <typename T, T v>
    constexpr cnumeric_type<T, v> cnumeric{};
}

#endif //! BOOST_HANA_TEST_TEST_CNUMERIC_CNUMERIC_HPP
