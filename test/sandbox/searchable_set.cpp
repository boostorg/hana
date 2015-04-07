/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include "searchable_set.hpp"

#include <boost/hana/assert.hpp>

#include <laws/base.hpp>
#include <test/cnumeric.hpp>
#include <test/numeric.hpp>
using namespace boost::hana;


template <int i>
constexpr auto n = test::numeric(i);

template <int i>
constexpr auto c = test::cnumeric<int, i>;

int main() {
    test::_injection<0> f{};
    test::_injection<1> g{};

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
                ap(singleton(f), singleton(c<0>)),
                singleton(f(c<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(singleton(f), doubleton(c<0>, c<1>)),
                doubleton(f(c<0>), f(c<1>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(doubleton(f, g), singleton(c<0>)),
                doubleton(f(c<0>), g(c<0>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(doubleton(f, g), doubleton(c<0>, c<1>)),
                union_(doubleton(f(c<0>), f(c<1>)),
                       doubleton(g(c<0>), g(c<1>)))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<SearchableSet>(c<0>),
                singleton(c<0>)
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
        // any_of
        {
            BOOST_HANA_CONSTEXPR_CHECK(any_of(singleton(n<0>), equal.to(n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(any_of(singleton(n<0>), equal.to(n<1>))));
            BOOST_HANA_CONSTEXPR_CHECK(any_of(doubleton(n<0>, n<1>), equal.to(n<0>)));
            BOOST_HANA_CONSTEXPR_CHECK(any_of(doubleton(n<0>, n<1>), equal.to(n<1>)));
            BOOST_HANA_CONSTEXPR_CHECK(not_(any_of(doubleton(n<0>, n<1>), equal.to(n<2>))));
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(find_if(singleton(c<0>), equal.to(c<0>)) == just(c<0>));
            BOOST_HANA_CONSTANT_CHECK(find_if(singleton(c<1>), equal.to(c<0>)) == nothing);

            BOOST_HANA_CONSTANT_CHECK(find_if(doubleton(c<0>, c<1>), equal.to(c<0>)) == just(c<0>));
            BOOST_HANA_CONSTANT_CHECK(find_if(doubleton(c<0>, c<1>), equal.to(c<1>)) == just(c<1>));
            BOOST_HANA_CONSTANT_CHECK(find_if(doubleton(c<0>, c<1>), equal.to(c<2>)) == nothing);
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
