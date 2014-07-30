/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/instantiates.hpp>
#include <boost/hana/core/typeclass.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <type_traits>
using namespace boost::hana;


struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
};

struct NotInstance;
struct Instance;
struct PredicatedInstance;

template <typename T>
struct Typeclass::instance<T, when<std::is_same<T, PredicatedInstance>{}>> { };

template <>
struct Typeclass::instance<Instance> { };


int main() {
    BOOST_HANA_STATIC_ASSERT(instantiates<Typeclass, Instance>);

    BOOST_HANA_STATIC_ASSERT(!instantiates<Typeclass, void>);
    BOOST_HANA_STATIC_ASSERT(!instantiates<Typeclass, NotInstance>);

    BOOST_HANA_STATIC_ASSERT(instantiates<Typeclass, PredicatedInstance>);
}
