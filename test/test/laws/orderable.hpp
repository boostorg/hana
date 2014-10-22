/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_ORDERABLE_HPP
#define BOOST_HANA_TEST_TEST_LAWS_ORDERABLE_HPP

#include <boost/hana/functional/infix.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


template <typename ...Orderables>
constexpr auto Orderable_laws(Orderables ...x) {
    using namespace boost::hana;

    auto implies = infix([](auto p, auto q) {
        return or_(not_(p), q);
    });

    auto incomparable = [](auto x, auto y) {
        return not_(or_(less(x, y), less(y, x)));
    };

    return and_([=](auto a) {
        return and_([=](auto b) {
            return and_([=](auto c) {

                return and_(
                    not_(less(a, a)),
                    less(a, b) ^implies^ not_(less(b, a)),
                    and_(less(a, b), less(b, c)) ^implies^ less(a, c),

                    and_(incomparable(a, b), incomparable(b, c))
                        ^implies^ incomparable(a, c)
                );

            }(x)...);
        }(x)...);
    }(x)...);
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_ORDERABLE_HPP
