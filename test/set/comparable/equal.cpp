/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto check = [](auto ...xs) {
    auto eq = [=](auto s) { return equal(to<Set>(s), set(xs...)); };
    return and_(
        all(permutations(list(xs...)), eq),
        not_(equal(set(xs...), set(xs..., x<999>)))
    );
};

BOOST_HANA_CONSTANT_ASSERT(check());
BOOST_HANA_CONSTANT_ASSERT(check(x<0>));
BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>));
BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>, x<2>));

int main() { (void)check; }
