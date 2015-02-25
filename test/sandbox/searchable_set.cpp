/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/searchable_set.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>
#include <test/numeric.hpp>

// instances
#include <test/auto/applicative.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/functor.hpp>
#include <test/auto/monad.hpp>
#include <test/auto/searchable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<SearchableSet> = make<Tuple>(
          type<Comparable>
        , type<Functor>
        , type<Applicative>
        , type<Monad>
        , type<Searchable>
    );

    template <>
    auto objects<SearchableSet> = make<Tuple>(
        singleton(numeric(0)),
        singleton(numeric(1)),
        doubleton(numeric(0), numeric(1)),
        doubleton(numeric(0), numeric(3)),
        doubleton(numeric(0), numeric(0))
    );
}}}


template <int i>
constexpr auto n = test::numeric(i);

template <int i>
constexpr auto c = test::cnumeric<int, i>;

int main() {
    test::check_datatype<SearchableSet>();
    using test::x;
    auto f = test::injection([]{});
    auto g = test::injection([]{});

    // union_
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(singleton(c<0>), singleton(c<0>)),
            singleton(c<0>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(singleton(c<0>), singleton(c<1>)),
            doubleton(c<0>, c<1>)
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(singleton(c<0>), doubleton(c<0>, c<1>)),
            doubleton(c<0>, c<1>)
        ));
    }

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(singleton(n<0>), singleton(n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(singleton(n<0>), singleton(n<1>))));

            BOOST_HANA_CONSTEXPR_CHECK(equal(singleton(n<0>), doubleton(n<0>, n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(singleton(n<0>), doubleton(n<0>, n<1>))));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(singleton(n<0>), doubleton(n<1>, n<1>))));

            BOOST_HANA_CONSTEXPR_CHECK(equal(doubleton(n<0>, n<1>), doubleton(n<0>, n<1>)));
            BOOST_HANA_CONSTEXPR_CHECK(equal(doubleton(n<0>, n<1>), doubleton(n<1>, n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(doubleton(n<0>, n<1>), doubleton(n<0>, n<0>))));
            BOOST_HANA_CONSTEXPR_CHECK(not_(equal(doubleton(n<0>, n<1>), doubleton(n<3>, n<4>))));
        }
    }

    // Functor
    {
        // transform
        {
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                transform(singleton(n<0>), f),
                singleton(f(n<0>))
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                transform(doubleton(n<0>, n<1>), f),
                doubleton(f(n<0>), f(n<1>))
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                transform(doubleton(n<0>, n<0>), f),
                singleton(f(n<0>))
            ));
        }
    }

    // Applicative
    {
        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(singleton(f), singleton(x<0>)),
                singleton(f(x<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(singleton(f), doubleton(x<0>, x<1>)),
                doubleton(f(x<0>), f(x<1>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(doubleton(f, g), singleton(x<0>)),
                doubleton(f(x<0>), g(x<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(doubleton(f, g), doubleton(x<0>, x<1>)),
                union_(doubleton(f(x<0>), f(x<1>)), doubleton(g(x<0>), g(x<1>)))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<SearchableSet>(x<0>),
                singleton(x<0>)
            ));
        }
    }

    // Monad
    {
        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(singleton(singleton(c<0>))),
                singleton(c<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(singleton(doubleton(c<0>, c<1>))),
                doubleton(c<0>, c<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(doubleton(singleton(c<0>), singleton(c<1>))),
                doubleton(c<0>, c<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(doubleton(doubleton(c<0>, c<1>), singleton(c<2>))),
                union_(doubleton(c<0>, c<1>), singleton(c<2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(doubleton(singleton(c<0>), doubleton(c<1>, c<2>))),
                union_(doubleton(c<0>, c<1>), singleton(c<2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(doubleton(doubleton(c<0>, c<1>), doubleton(c<2>, c<3>))),
                union_(doubleton(c<0>, c<1>), doubleton(c<2>, c<3>))
            ));
        }
    }

    // Searchable
    {

        BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto x) {
            return [=](auto y) { return equal(x, y); };
        };

        // any
        {
            BOOST_HANA_CONSTEXPR_CHECK(any(singleton(n<0>), is(n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(any(singleton(n<0>), is(n<1>))));
            BOOST_HANA_CONSTEXPR_CHECK(any(doubleton(n<0>, n<1>), is(n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(any(doubleton(n<0>, n<1>), is(n<1>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(any(doubleton(n<0>, n<1>), is(n<2>))));
        }

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(find(singleton(c<0>), is(c<0>)) == just(c<0>));
            BOOST_HANA_CONSTANT_CHECK(find(singleton(c<1>), is(c<0>)) == nothing);

            BOOST_HANA_CONSTANT_CHECK(find(doubleton(c<0>, c<1>), is(c<0>)) == just(c<0>));
            BOOST_HANA_CONSTANT_CHECK(find(doubleton(c<0>, c<1>), is(c<1>)) == just(c<1>));
            BOOST_HANA_CONSTANT_CHECK(find(doubleton(c<0>, c<1>), is(c<2>)) == nothing);
        }

        // subset
        {
            BOOST_HANA_CONSTEXPR_CHECK(subset(singleton(n<0>), singleton(n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(subset(singleton(n<1>), singleton(n<0>))));

            BOOST_HANA_CONSTEXPR_CHECK(subset(singleton(n<0>), doubleton(n<0>, n<1>)));
            BOOST_HANA_CONSTEXPR_CHECK(subset(singleton(n<1>), doubleton(n<0>, n<1>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(subset(singleton(n<2>), doubleton(n<0>, n<1>))));

            BOOST_HANA_CONSTEXPR_CHECK(subset(doubleton(n<0>, n<1>), doubleton(n<0>, n<1>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(subset(doubleton(n<0>, n<2>), doubleton(n<0>, n<1>))));
            BOOST_HANA_CONSTEXPR_CHECK(not_(subset(doubleton(n<2>, n<3>), doubleton(n<0>, n<1>))));
        }
    }
}
