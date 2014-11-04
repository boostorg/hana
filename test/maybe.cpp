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

#include <test/auto/base.hpp>
#include <test/identity.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Maybe> = tuple(
        type<Comparable>,

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
        just(x<0>),
        just(x<1>),
        just(x<2>)
    );
}}}


int main() {
    test::check_datatype<Maybe>();

    // Maybe methods
    {
        auto f = test::injection([]{});
        auto x = test::injection([]{})();
        auto y = test::injection([]{})();
        constexpr struct { } undefined{};

        // maybe
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(maybe(x, undefined, nothing), x));
            BOOST_HANA_CONSTANT_ASSERT(equal(maybe(undefined, f, just(x)), f(x)));
        }

        // is_nothing
        {
            BOOST_HANA_CONSTANT_ASSERT(is_nothing(nothing));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_nothing(just(undefined))));
        }

        // is_just
        {
            BOOST_HANA_CONSTANT_ASSERT(is_just(just(undefined)));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_just(nothing)));
        }

        // from_just
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(from_just(just(x)), x));
            // from_just(nothing);
        }

        // from_maybe
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(from_maybe(x, nothing), x));
            BOOST_HANA_CONSTANT_ASSERT(equal(from_maybe(undefined, just(y)), y));
        }

        // only_when
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                only_when(always(true_), f, x),
                just(f(x))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                only_when(always(false_), f, x),
                nothing
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
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

            BOOST_HANA_CONSTANT_ASSERT(equal(nothing, nothing));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(nothing, just(x))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(just(x), nothing)));
            BOOST_HANA_CONSTANT_ASSERT(equal(just(x), just(x)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(just(x), just(y))));
        }
    }

    // Functor
    {
        // fmap
        {
            auto f = test::injection([]{});
            auto x = test::injection([]{})();

            BOOST_HANA_CONSTANT_ASSERT(equal(fmap(nothing, f), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(fmap(just(x), f), just(f(x))));
        }
    }

    // Applicative
    {
        auto f = test::injection([]{});
        auto x = test::injection([]{})();

        // ap
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(ap(nothing, nothing), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(ap(just(f), nothing), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(ap(nothing, just(x)), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(ap(just(f), just(x)), just(f(x))));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(lift<Maybe>(x), just(x)));
        }
    }

    // Monad
    {
        // flatten
        {
            auto x = test::injection([]{})();

            BOOST_HANA_CONSTANT_ASSERT(equal(flatten(nothing), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(flatten(just(nothing)), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(flatten(just(just(x))), just(x)));
        }
    }

    // Traversable
    {
        auto f = test::injection([]{});
        auto x = test::injection([]{})();

        auto applicative = ::test::identity;
        using A = ::test::Identity;

        // traverse
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                traverse<A>(just(x), compose(applicative, f)),
                applicative(just(f(x)))
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
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
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(just(x), is(x)),
                just(x)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(just(x), is(y)),
                nothing
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(nothing, is(x)),
                nothing
            ));
        }

        // any
        {
            BOOST_HANA_CONSTANT_ASSERT(any(just(x), is(x)));
            BOOST_HANA_CONSTANT_ASSERT(not_(any(just(x), is(y))));
            BOOST_HANA_CONSTANT_ASSERT(not_(any(nothing, is(x))));
        }
    }

    // Foldable
    {
        auto x = test::injection([]{})();
        auto s = test::injection([]{})();
        auto f = test::injection([]{});

        // foldl
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(foldl(just(x), s, f), f(s, x)));
            BOOST_HANA_CONSTANT_ASSERT(equal(foldl(nothing, s, f), s));
        }

        // foldr
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(foldr(just(x), s, f), f(x, s)));
            BOOST_HANA_CONSTANT_ASSERT(equal(foldr(nothing, s, f), s));
        }
    }
}
