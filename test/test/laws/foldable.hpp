/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_FOLDABLE_HPP
#define BOOST_HANA_TEST_TEST_LAWS_FOLDABLE_HPP

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>


template <typename ...Foldables>
constexpr auto Foldable_laws(Foldables&& ...xs) {
    using namespace boost::hana;
    auto f = test::injection([]{});
    auto s = test::injection([]{})();

    return and_(
        and_(
            equal(foldl(xs, s, f), foldl(to<Tuple>(xs), s, f)),
            equal(foldr(xs, s, f), foldr(to<Tuple>(xs), s, f))
        )...
    );
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_FOLDABLE_HPP
