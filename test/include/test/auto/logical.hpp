/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_LOGICAL_HPP
#define BOOST_HANA_TEST_TEST_AUTO_LOGICAL_HPP

#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename L>
    auto laws<Logical, L> = [] {

    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_LOGICAL_HPP
