/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/typeclass.hpp>

#include <boost/hana/assert.hpp>
using namespace boost::hana;


struct Disabled;

struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
};

// Instance enabled for all data types.
template <typename T> struct Typeclass::instance<T> { };

// Instance explicitly disabled for `Disabled`.
template <> struct Typeclass::instance<Disabled> : disable { };


int main() {
    BOOST_HANA_CONSTANT_ASSERT(is_a<Typeclass, struct AnythingElse>);
    BOOST_HANA_CONSTANT_ASSERT(!is_a<Typeclass, Disabled>);
}
