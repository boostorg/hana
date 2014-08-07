/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/identity.hpp>

#include <boost/hana/detail/assert.hpp>

#include "unique_comparable.hpp"
#include "unique_function.hpp"
using namespace boost::hana;


auto x = unique_comparable([]{});
auto f = unique_function([]{});

BOOST_HANA_CONSTANT_ASSERT(equal(lift<Identity>(x), identity(x)));
BOOST_HANA_CONSTANT_ASSERT(equal(ap(identity(f), identity(x)), identity(f(x))));

int main() { }
