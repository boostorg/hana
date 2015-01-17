/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/models.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/tuple.hpp>

#include <type_traits>
using namespace boost::hana;


struct Typeclass;
struct NotInstance;
struct Instance;
namespace boost { namespace hana {
    template <>
    struct models_impl< ::Typeclass> {
        template <typename T, typename Context>
        static constexpr bool apply = std::is_same<T, ::Instance>::value;
    };
}}

struct an_instance { struct hana { using datatype = Instance; }; };
struct not_instance { struct hana { using datatype = NotInstance; }; };


int main() {
    // standard syntax
    BOOST_HANA_CONSTANT_CHECK(models<Typeclass, Instance>);
    BOOST_HANA_CONSTANT_CHECK(!models<Typeclass, NotInstance>);
    BOOST_HANA_CONSTANT_CHECK(!models<Typeclass, void>);

    // alternate syntax
    BOOST_HANA_CONSTANT_CHECK(is_a<Typeclass>(an_instance{}));
    BOOST_HANA_CONSTANT_CHECK(is_an<Typeclass>(an_instance{}));

    BOOST_HANA_CONSTANT_CHECK(!is_a<Typeclass>(not_instance{}));
    BOOST_HANA_CONSTANT_CHECK(!is_an<Typeclass>(not_instance{}));

    BOOST_HANA_CONSTANT_CHECK(!is_a<Typeclass>(1));
    BOOST_HANA_CONSTANT_CHECK(!is_an<Typeclass>(1));

    BOOST_HANA_CONSTANT_CHECK(!is_a<Typeclass>('2'));
    BOOST_HANA_CONSTANT_CHECK(!is_an<Typeclass>('2'));

    BOOST_HANA_CONSTANT_CHECK(is_a<Instance>(an_instance{}));
    BOOST_HANA_CONSTANT_CHECK(is_an<Instance>(an_instance{}));
    BOOST_HANA_CONSTANT_CHECK(is_a<NotInstance>(not_instance{}));

    // check with a couple of "real" instances
    BOOST_HANA_CONSTANT_CHECK(is_an<Iterable>(tuple(1, '2', 3)));
    BOOST_HANA_CONSTANT_CHECK(is_a<Monad>(tuple(1, '2', 3)));
    BOOST_HANA_CONSTANT_CHECK(is_a<Foldable>(tuple(1, '2', 3)));

    BOOST_HANA_CONSTANT_CHECK(!is_an<Iterable>(just(1)));
    BOOST_HANA_CONSTANT_CHECK(!is_an<Iterable>(nothing));
    BOOST_HANA_CONSTANT_CHECK(is_a<Foldable>(just('1')));
    BOOST_HANA_CONSTANT_CHECK(is_a<Foldable>(nothing));
    BOOST_HANA_CONSTANT_CHECK(is_a<Monad>(just("abcd")));
    BOOST_HANA_CONSTANT_CHECK(is_a<Monad>(nothing));

    BOOST_HANA_CONSTANT_CHECK(is<Orderable>(1));

    BOOST_HANA_CONSTANT_CHECK(is_a<Tuple>(tuple(1, '2', 3)));
    BOOST_HANA_CONSTANT_CHECK(is_an<int>(1));
}
