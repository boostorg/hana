/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP
#define BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP

#include <boost/hana/constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename C>
    auto laws<Constant, C> = [] {
        BOOST_HANA_CONSTANT_CHECK(models<Constant, C>);

        for_each(objects<C>, [](auto c) {
            constexpr auto must_be_constexpr = value(c);
            (void)must_be_constexpr;
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_CONSTANT_HPP
