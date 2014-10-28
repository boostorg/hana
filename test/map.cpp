/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/injection.hpp>
#include <test/minimal_product.hpp>
#include <test/minimal_record.hpp>
#include <test/seq.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/searchable.hpp>
using namespace boost::hana;


template <int i>
auto key = test::x<i>;

template <int i>
auto value = test::x<-i>;

template <int i, int j>
auto p = test::minimal_product(key<i>, value<j>);

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Map> = tuple(
        type<Searchable>,
        type<Comparable>
    );

    template <>
    auto objects<Map> = tuple(
        map(),
        map(p<1, 1>),
        map(p<1, 2>),
        map(p<1, 1>, p<2, 2>)
    );
}}}


int main() {
    test::check_datatype<Map>();

    // Map interface
    {
        auto list = test::seq;
        auto foldable = test::seq;
        auto record = test::minimal_record;
        using L = test::Seq;
        using test::x;

        // keys
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                keys(map()),
                list()
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                keys(map(p<1, 1>)),
                list(key<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                keys(map(p<1, 1>, p<2, 2>)),
                list(key<1>, key<2>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                keys(map(p<1, 1>, p<2, 2>, p<3, 3>)),
                list(key<1>, key<2>, key<3>)
            ));
        }

        // values
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                values(map()),
                list()
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                values(map(p<1, 1>)),
                list(value<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                values(map(p<1, 1>, p<2, 2>)),
                list(value<1>, value<2>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                values(map(p<1, 1>, p<2, 2>, p<3, 3>)),
                list(value<1>, value<2>, value<3>)
            ));
        }

        // Conversions
        {
            // Record -> Map
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<Map>(record(test::x<1>, test::x<2>)),
                    map(pair(test::member1, test::x<1>),
                        pair(test::member2, test::x<2>))
                ));
            }

            // Foldable -> Map
            {
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<Map>(foldable()),
                    map()
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<Map>(foldable(p<1, 1>)),
                    map(p<1, 1>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<Map>(foldable(p<1, 1>, p<2, 2>)),
                    map(p<1, 1>, p<2, 2>)
                ));
                BOOST_HANA_CONSTANT_ASSERT(equal(
                    to<Map>(foldable(p<1, 1>, p<2, 2>, p<3, 3>)),
                    map(p<1, 1>, p<2, 2>, p<3, 3>)
                ));
            }

            // Map -> List
            {
                BOOST_HANA_CONSTEXPR_LAMBDA auto check = [=](auto ...xs) {
                    BOOST_HANA_CONSTANT_ASSERT(
                        elem(permutations(list(xs...)), to<L>(map(xs...)))
                    );
                };
                check();
                check(p<1, 1>);
                check(p<1, 1>, p<2, 2>);
                check(p<1, 1>, p<2, 2>, p<3, 3>);
                check(p<1, 1>, p<2, 2>, p<3, 3>, p<4, 4>);
            }
        }
    }

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(map(), map()));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map())));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(), map(p<1, 1>))));

            BOOST_HANA_CONSTANT_ASSERT(equal(map(p<1, 1>), map(p<1, 1>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map(p<1, 2>))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map(p<2, 1>))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>), map(p<1, 1>, p<2, 2>))));

            BOOST_HANA_CONSTANT_ASSERT(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<2, 2>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(map(p<1, 1>, p<2, 2>), map(p<2, 2>, p<1, 1>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<9, 1>, p<2, 2>))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 9>, p<2, 2>))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<9, 2>))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<2, 9>))));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(map(p<1, 1>, p<2, 2>), map(p<1, 1>, p<2, 2>, p<3, 3>))));
        }
    }

    // Searchable
    {
        auto is = [](auto x) {
            return [=](auto y) { return equal(x, y); };
        };

        // any
        {
            BOOST_HANA_CONSTANT_ASSERT(not_(any(map(), is(key<1>))));

            BOOST_HANA_CONSTANT_ASSERT(any(map(p<1, 1>), is(key<1>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(any(map(p<1, 1>), is(key<2>))));

            BOOST_HANA_CONSTANT_ASSERT(any(map(p<1, 1>, p<2, 2>), is(key<1>)));
            BOOST_HANA_CONSTANT_ASSERT(any(map(p<1, 1>, p<2, 2>), is(key<2>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(any(map(p<1, 1>, p<2, 2>), is(key<3>))));
        }

        // find
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(map(), is(key<1>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(map(p<1, 1>), is(key<1>)),
                just(value<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(map(p<1, 1>), is(key<2>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(map(p<1, 1>, p<2, 2>), is(key<1>)),
                just(value<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(map(p<1, 1>, p<2, 2>), is(key<2>)),
                just(value<2>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(map(p<1, 1>, p<2, 2>), is(key<3>)),
                nothing
            ));
        }
    }
}
