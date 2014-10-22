/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_LAWS_CONSTANT_HPP

#include <boost/hana/constant.hpp>


template <typename ...Constants>
constexpr auto Constant_laws(Constants ...cs) {
    using namespace boost::hana;

    auto check = [](auto c) {
        constexpr auto must_be_constexpr = value(c);
        (void)must_be_constexpr;
    };

    int check_all[] = {(check(cs), 1)...};
    (void)check_all;
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_CONSTANT_HPP
