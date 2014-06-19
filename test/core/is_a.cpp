/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
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

template <>
struct Typeclass<Instance> : defaults<Typeclass>::with<Instance> { };


struct an_instance { using hana_datatype = Instance; };
struct not_instance { using hana_datatype = NotInstance; };


int main() {
    BOOST_HANA_STATIC_ASSERT(is_a<Typeclass>(an_instance{}));
    BOOST_HANA_STATIC_ASSERT(is_an<Typeclass>(an_instance{}));

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass>(not_instance{}));
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass>(not_instance{}));

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass>(1));
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass>(1));

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass>('2'));
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass>('2'));

    // Check with a couple of "real" instances
    BOOST_HANA_STATIC_ASSERT(is_an<Iterable>(list(1, '2', 3)));
    BOOST_HANA_STATIC_ASSERT(is_a<Monad>(list(1, '2', 3)));

    BOOST_HANA_STATIC_ASSERT(!is_an<Iterable>(just(1)));
    BOOST_HANA_STATIC_ASSERT(!is_an<Iterable>(nothing));
    BOOST_HANA_STATIC_ASSERT(is_a<Monad>(just("abcd")));
    BOOST_HANA_STATIC_ASSERT(is_a<Monad>(nothing));
}
