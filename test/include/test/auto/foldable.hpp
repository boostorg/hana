/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_FOLDABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_FOLDABLE_HPP

#include <boost/hana/foldable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>


namespace boost { namespace hana { namespace test {
    template <typename T>
    auto laws<Foldable, T> = [] {
        auto f = injection([]{});
        auto s = injection([]{})();

        for_each(objects<T>, [=](auto xs) {
            BOOST_HANA_CHECK(equal(
                foldl(xs, s, f),
                foldl(to<Tuple>(xs), s, f)
            ));

            BOOST_HANA_CHECK(equal(
                foldr(xs, s, f),
                foldr(to<Tuple>(xs), s, f)
            ));
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_FOLDABLE_HPP
