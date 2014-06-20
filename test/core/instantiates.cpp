/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


//////////////////////////////////////////////////////////////////////////////
// Type class
//////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Typeclass;

namespace boost { namespace hana {
    template <>
    struct instance<Typeclass> {
        template <typename T> struct with { };
    };

    template <>
    struct defaults<Typeclass> {
        template <typename T> struct with : defaults<> { };
    };
}}

template <typename T>
struct Typeclass : instance<Typeclass>::template with<T> { };


//////////////////////////////////////////////////////////////////////////////
// Data types
//////////////////////////////////////////////////////////////////////////////
struct NotInstance;
struct Instance;
struct ComplimentaryInstance;

namespace boost { namespace hana {
    template <>
    struct instance<Typeclass>::with<ComplimentaryInstance>
        : defaults<Typeclass>::with<ComplimentaryInstance>
    { };
}}

template <>
struct Typeclass<Instance> : defaults<Typeclass>::with<Instance> { };


int main() {
    BOOST_HANA_STATIC_ASSERT(instantiates<Typeclass, Instance>);

    BOOST_HANA_STATIC_ASSERT(!instantiates<Typeclass, void>);
    BOOST_HANA_STATIC_ASSERT(!instantiates<Typeclass, NotInstance>);

    BOOST_HANA_STATIC_ASSERT(instantiates<Typeclass, ComplimentaryInstance>);
}
