/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_LIST_HPP
#define BOOST_HANA_TEST_TEST_LAWS_LIST_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/tuple.hpp>


template <typename ...Lists>
constexpr auto List_laws(Lists ...lists) {
    using namespace boost::hana;
    auto iff = infix([](auto p, auto q) { return or_(not_(p), q); });

    return and_([=](auto xs) {
        return and_([=](auto ys) {

            return equal(xs, ys) ^iff^ equal(to<Tuple>(xs), to<Tuple>(ys));

        }(lists)...);
    }(lists)...);
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_LIST_HPP
