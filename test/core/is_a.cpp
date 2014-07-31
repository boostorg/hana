/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/typeclass.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad/monad.hpp>

#include <type_traits>
using namespace boost::hana;


struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
};

struct NotInstance;
struct Instance;
struct PredicatedInstance;

template <> struct Typeclass::instance<Instance> { };
template <typename T>
struct Typeclass::instance<T, when<std::is_same<T, PredicatedInstance>{}>> { };

struct an_instance { using hana_datatype = Instance; };
struct not_instance { using hana_datatype = NotInstance; };


int main() {
    // standard syntax
    BOOST_HANA_STATIC_ASSERT(is_a<Typeclass, Instance>);
    BOOST_HANA_STATIC_ASSERT(is_an<Typeclass, Instance>);

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass, void>);
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass, void>);

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass, NotInstance>);
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass, NotInstance>);

    BOOST_HANA_STATIC_ASSERT(is_a<Typeclass, PredicatedInstance>);
    BOOST_HANA_STATIC_ASSERT(is_an<Typeclass, PredicatedInstance>);


    // alternate syntax
    BOOST_HANA_STATIC_ASSERT(is_a<Typeclass>(an_instance{}));
    BOOST_HANA_STATIC_ASSERT(is_an<Typeclass>(an_instance{}));

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass>(not_instance{}));
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass>(not_instance{}));

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass>(1));
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass>(1));

    BOOST_HANA_STATIC_ASSERT(!is_a<Typeclass>('2'));
    BOOST_HANA_STATIC_ASSERT(!is_an<Typeclass>('2'));

    // check with a couple of "real" instances
    BOOST_HANA_STATIC_ASSERT(is_an<Iterable>(list(1, '2', 3)));
    BOOST_HANA_STATIC_ASSERT(is_a<Monad>(list(1, '2', 3)));
    BOOST_HANA_STATIC_ASSERT(is_a<Foldable>(list(1, '2', 3)));

    BOOST_HANA_STATIC_ASSERT(!is_an<Iterable>(just(1)));
    BOOST_HANA_STATIC_ASSERT(!is_an<Iterable>(nothing));
    BOOST_HANA_STATIC_ASSERT(is_a<Foldable>(just('1')));
    BOOST_HANA_STATIC_ASSERT(is_a<Foldable>(nothing));
    BOOST_HANA_STATIC_ASSERT(is_a<Monad>(just("abcd")));
    BOOST_HANA_STATIC_ASSERT(is_a<Monad>(nothing));
}
