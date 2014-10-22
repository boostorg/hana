/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_CNUMERIC_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_CNUMERIC_CONSTANT_HPP

#include <boost/hana/constant.hpp>
#include <test/cnumeric/cnumeric.hpp>


namespace boost { namespace hana {
    template <>
    struct Constant::instance<test::CNumeric>
        : Constant::mcd
    {
        template <typename X>
        static constexpr auto value_impl(X const&) {
            return X::value;
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_CNUMERIC_CONSTANT_HPP
