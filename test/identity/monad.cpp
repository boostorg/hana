/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/identity.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/monad/laws.hpp>

#include "unique_comparable.hpp"
#include "unique_function.hpp"
using namespace boost::hana;


auto x = unique_comparable([]{});
auto y = unique_comparable([]{});
auto f = compose(identity, unique_function([]{}));
auto g = compose(identity, unique_function([]{}));

BOOST_HANA_CONSTANT_ASSERT(equal(flatten(identity(identity(x))), identity(x)));
BOOST_HANA_CONSTANT_ASSERT(Monad::laws::check(identity(x), y, f, g));

int main() { }
