/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_LAWS_COMPARABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/logical.hpp>


template <typename ...Comparables>
constexpr auto Comparable_laws(Comparables ...x) {
    using namespace boost::hana;

    auto implies = infix([](auto p, auto q) { return or_(not_(p), q); });
    return and_([=](auto a) {
        return and_([=](auto b) {
            return and_([=](auto c) {

                return and_(
                    equal(a, a),
                    equal(a, b) ^implies^ equal(b, a),
                    and_(equal(a, b), equal(b, c)) ^implies^ equal(a, c)
                );

            }(x)...);
        }(x)...);
    }(x)...);
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_COMPARABLE_HPP
