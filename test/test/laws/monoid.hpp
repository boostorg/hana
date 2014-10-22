/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_MONOID_HPP
#define BOOST_HANA_TEST_TEST_LAWS_MONOID_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>


template <typename M, typename ...Monoid>
constexpr auto Monoid_laws(Monoid ...m) {
    using namespace boost::hana;

    return and_([=](auto x) {
        return and_([=](auto y) {
            return and_([=](auto z) {

                return and_(
                    equal(plus(zero<M>, x), x),
                    equal(plus(x, zero<M>), x),
                    equal(plus(x, plus(y, z)), plus(plus(x, y), z))
                );

            }(m)...);
        }(m)...);
    }(m)...);
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_MONOID_HPP
