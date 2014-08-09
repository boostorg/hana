/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/identity.hpp>

#include <boost/hana/comparable/detail/laws.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>

#include "unique_comparable.hpp"
using namespace boost::hana;


auto implies = infix([](auto p, auto q) {
    return or_(not_(p), q);
});

auto iff = infix([](auto p, auto q) {
    return and_(p ^implies^ q, q ^implies^ p);
});

auto check = [](auto x, auto y) {
    return equal(x, y) ^iff^ equal(identity(x), identity(y));
};

auto x = unique_comparable([]{});
auto y = unique_comparable([]{});
auto z = unique_comparable([]{});

BOOST_HANA_CONSTANT_ASSERT(
    all_of(ap(list(check), list(x,y,z), list(x,y,z)))
);

BOOST_HANA_CONSTANT_ASSERT(Comparable::laws::check(
    list(identity(x), identity(y), identity(z))
));

int main() { (void)check; }
