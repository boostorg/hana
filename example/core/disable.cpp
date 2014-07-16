/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/list/list.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<StdTuple> : disable { };
}}

int main() {
    // Usually, List should provide Foldable.
    BOOST_HANA_STATIC_ASSERT(instantiates<List, StdTuple>);
    BOOST_HANA_STATIC_ASSERT(!instantiates<Foldable, StdTuple>);
}
