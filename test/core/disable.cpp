/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct Disabled;

struct Typeclass;
namespace boost { namespace hana {
    BOOST_HANA_TYPECLASS_BOILERPLATE(::Typeclass)
}}
struct Typeclass : typeclass<Typeclass> { };

// Instance enabled for all data types.
template <typename T> struct Typeclass::instance<T> { };

// Instance explicitly disabled for `Disabled`.
template <> struct Typeclass::instance<Disabled> : disable { };


int main() {
    BOOST_HANA_STATIC_ASSERT(instantiates<Typeclass, struct AnythingElse>);
    BOOST_HANA_STATIC_ASSERT(!instantiates<Typeclass, Disabled>);
}
