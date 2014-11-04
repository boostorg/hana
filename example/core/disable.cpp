/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/disable.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/list.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<ext::std::Tuple> : disable { };
}}

// Usually, List should provide Foldable.
BOOST_HANA_CONSTANT_ASSERT(is_a<List, ext::std::Tuple>);
BOOST_HANA_CONSTANT_ASSERT(!is_a<Foldable, ext::std::Tuple>);

int main() { }
