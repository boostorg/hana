/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/maybe.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/applicative.hpp>
#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/functor.hpp>
#include <test/auto/monad.hpp>
#include <test/auto/orderable.hpp>
#include <test/auto/searchable.hpp>
#include <test/auto/traversable.hpp>
#include <test/cnumeric.hpp>
#include <test/identity.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Maybe> = tuple(
        type<Comparable>,
        type<Orderable>,

        type<Functor>,
        type<Applicative>,
        type<Monad>,
        type<Traversable>,

        type<Foldable>,
        type<Searchable>
    );

    template <>
    auto objects<Maybe> = tuple(
        nothing,
        just(ord<0>),
        just(ord<1>),
        just(ord<2>)
    );
}}}


int main() {
    test::check_datatype<Maybe>();
    constexpr struct { } undefined{};

    // Maybe methods
    {
        auto f = test::injection([]{});
        auto x = test::injection([]{})();
        auto y = test::injection([]{})();

        // maybe
        {
            BOOST_HANA_CONSTANT_CHECK(equal(maybe(x, undefined, nothing), x));
            BOOST_HANA_CONSTANT_CHECK(equal(maybe(undefined, f, just(x)), f(x)));
        }

        // is_nothing
        {
            BOOST_HANA_CONSTANT_CHECK(is_nothing(nothing));
            BOOST_HANA_CONSTANT_CHECK(not_(is_nothing(just(undefined))));
        }

        // is_just
        {
            BOOST_HANA_CONSTANT_CHECK(is_just(just(undefined)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_just(nothing)));
        }

        // from_just
        {
            BOOST_HANA_CONSTANT_CHECK(equal(from_just(just(x)), x));
            // from_just(nothing);
        }

        // from_maybe
        {
            BOOST_HANA_CONSTANT_CHECK(equal(from_maybe(x, nothing), x));
            BOOST_HANA_CONSTANT_CHECK(equal(from_maybe(undefined, just(y)), y));
        }

        // only_when
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                only_when(always(true_), f, x),
                just(f(x))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                only_when(always(false_), f, x),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                only_when(always(false_), undefined, x),
                nothing
            ));
        }
    }

    // Comparable
    {
        // equal
        {
            auto x = test::injection([]{})();
            auto y = test::injection([]{})();

            BOOST_HANA_CONSTANT_CHECK(equal(nothing, nothing));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(nothing, just(x))));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(just(x), nothing)));
            BOOST_HANA_CONSTANT_CHECK(equal(just(x), just(x)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(just(x), just(y))));
        }
    }

    // Orderable
    {
        // less
        {
            BOOST_HANA_CONSTANT_CHECK(less(nothing, just(undefined)));
            BOOST_HANA_CONSTANT_CHECK(not_(less(just(undefined), nothing)));
            BOOST_HANA_CONSTANT_CHECK(not_(less(nothing, nothing)));

            BOOST_HANA_CONSTANT_CHECK(less(just(ord<0>), just(ord<1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(less(just(ord<0>), just(ord<0>))));
            BOOST_HANA_CONSTANT_CHECK(not_(less(just(ord<1>), just(ord<0>))));
        }
    }

    // Functor
    {
        // fmap
        {
            auto f = test::injection([]{});
            auto x = test::injection([]{})();

            BOOST_HANA_CONSTANT_CHECK(equal(fmap(nothing, f), nothing));
            BOOST_HANA_CONSTANT_CHECK(equal(fmap(just(x), f), just(f(x))));
        }
    }

    // Applicative
    {
        auto f = test::injection([]{});
        auto x = test::injection([]{})();

        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(ap(nothing, nothing), nothing));
            BOOST_HANA_CONSTANT_CHECK(equal(ap(just(f), nothing), nothing));
            BOOST_HANA_CONSTANT_CHECK(equal(ap(nothing, just(x)), nothing));
            BOOST_HANA_CONSTANT_CHECK(equal(ap(just(f), just(x)), just(f(x))));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(lift<Maybe>(x), just(x)));
        }
    }

    // Monad
    {
        // flatten
        {
            auto x = test::injection([]{})();

            BOOST_HANA_CONSTANT_CHECK(equal(flatten(nothing), nothing));
            BOOST_HANA_CONSTANT_CHECK(equal(flatten(just(nothing)), nothing));
            BOOST_HANA_CONSTANT_CHECK(equal(flatten(just(just(x))), just(x)));
        }
    }

    // Traversable
    {
        auto f = test::injection([]{});
        auto x = test::injection([]{})();

        auto applicative = test::identity;
        using A = test::Identity;

        // traverse
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                traverse<A>(just(x), compose(applicative, f)),
                applicative(just(f(x)))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                traverse<A>(nothing, compose(applicative, f)),
                applicative(nothing)
            ));
        }
    }

    // Searchable
    {
        auto x = test::injection([]{})();
        auto y = test::injection([]{})();
        auto is = [](auto x) {
            return [=](auto y) { return equal(x, y); };
        };

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(just(x), is(x)),
                just(x)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(just(x), is(y)),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(nothing, is(x)),
                nothing
            ));

            // Previously, there was a bug that would make this fail.
            auto non_const_nothing = nothing;
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(non_const_nothing, is(x)),
                nothing
            ));
        }

        // any
        {
            BOOST_HANA_CONSTANT_CHECK(any(just(x), is(x)));
            BOOST_HANA_CONSTANT_CHECK(not_(any(just(x), is(y))));
            BOOST_HANA_CONSTANT_CHECK(not_(any(nothing, is(x))));
        }
    }

    // Foldable
    {
        auto x = test::injection([]{})();
        auto s = test::injection([]{})();
        auto f = test::injection([]{});

        // foldl
        {
            BOOST_HANA_CONSTANT_CHECK(equal(foldl(just(x), s, f), f(s, x)));
            BOOST_HANA_CONSTANT_CHECK(equal(foldl(nothing, s, f), s));
        }

        // foldr
        {
            BOOST_HANA_CONSTANT_CHECK(equal(foldr(just(x), s, f), f(x, s)));
            BOOST_HANA_CONSTANT_CHECK(equal(foldr(nothing, s, f), s));
        }

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(unpack(nothing, f), f()));
            BOOST_HANA_CONSTANT_CHECK(equal(unpack(just(x), f), f(x)));

            // Previously, there was a bug that would make this fail.
            auto non_const_nothing = nothing;
            BOOST_HANA_CONSTANT_CHECK(equal(unpack(non_const_nothing, f), f()));
        }
    }
}
