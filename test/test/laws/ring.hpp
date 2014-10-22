/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_RING_HPP
#define BOOST_HANA_TEST_TEST_LAWS_RING_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/ring.hpp>


template <typename R, typename ...Ring>
constexpr auto Ring_laws(Ring ...r) {
    using namespace boost::hana;

    return and_([=](auto x) {
        return and_([=](auto y) {
            return and_([=](auto z) {

                return and_(
                    equal(mult(x, mult(y, z)), mult(mult(x, y), z)),
                    equal(mult(one<R>, x), x),
                    equal(mult(x, one<R>), x),
                    equal(mult(x, plus(y, z)), plus(mult(x, y), mult(x, z)))
                );

            }(r)...);
        }(r)...);
    }(r)...);
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_RING_HPP
