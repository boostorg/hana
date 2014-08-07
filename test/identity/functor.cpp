/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/identity.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functor/laws.hpp>
#include <boost/hana/list/instance.hpp>

#include "unique_comparable.hpp"
#include "unique_function.hpp"
using namespace boost::hana;


auto x = unique_comparable([]{});
auto y = unique_comparable([]{});
auto z = unique_comparable([]{});

auto f = unique_function([]{});
auto g = unique_function([]{});
auto h = unique_function([]{});
auto i = unique_function([]{});

BOOST_HANA_CONSTANT_ASSERT(
    equal(fmap(f, identity(x)), identity(f(x)))
);

BOOST_HANA_CONSTANT_ASSERT(Functor::laws::check(
    list(identity(x), identity(y), identity(z)),
    list(f, g),
    list(h, i)
));

int main() { }
