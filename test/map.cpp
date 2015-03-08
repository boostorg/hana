/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/assert.hpp>
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
auto val = test::x<-i>;

template <int i, int j>
auto p = test::minimal_product(key<i>, val<j>);

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Map> = make<Tuple>(
        type<Searchable>,
        type<Comparable>
    );

    template <>
    auto objects<Map> = make<Tuple>(
        make<Map>(),
        make<Map>(p<1, 1>),
        make<Map>(p<1, 2>),
        make<Map>(p<1, 1>, p<2, 2>)
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
            BOOST_HANA_CONSTANT_CHECK(equal(
                keys(make<Map>()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                keys(make<Map>(p<1, 1>)),
                list(key<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                keys(make<Map>(p<1, 1>, p<2, 2>)),
                list(key<1>, key<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                keys(make<Map>(p<1, 1>, p<2, 2>, p<3, 3>)),
                list(key<1>, key<2>, key<3>)
            ));
        }

        // values
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                values(make<Map>()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                values(make<Map>(p<1, 1>)),
                list(val<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                values(make<Map>(p<1, 1>, p<2, 2>)),
                list(val<1>, val<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                values(make<Map>(p<1, 1>, p<2, 2>, p<3, 3>)),
                list(val<1>, val<2>, val<3>)
            ));
        }

        // Conversions
        {
            // Record -> Map
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<Map>(record(test::x<1>, test::x<2>)),
                    make<Map>(pair(test::member1, test::x<1>),
                        pair(test::member2, test::x<2>))
                ));
            }

            // Foldable -> Map
            {
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<Map>(foldable()),
                    make<Map>()
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<Map>(foldable(p<1, 1>)),
                    make<Map>(p<1, 1>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<Map>(foldable(p<1, 1>, p<2, 2>)),
                    make<Map>(p<1, 1>, p<2, 2>)
                ));
                BOOST_HANA_CONSTANT_CHECK(equal(
                    to<Map>(foldable(p<1, 1>, p<2, 2>, p<3, 3>)),
                    make<Map>(p<1, 1>, p<2, 2>, p<3, 3>)
                ));
            }

            // Map -> Sequence
            {
                BOOST_HANA_CONSTEXPR_LAMBDA auto check = [=](auto ...xs) {
                    BOOST_HANA_CONSTANT_CHECK(
                        elem(permutations(list(xs...)), to<L>(make<Map>(xs...)))
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
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(),
                make<Map>()
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(),
                make<Map>(p<1, 1>)
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<1, 1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<1, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<2, 1>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>),
                make<Map>(p<1, 1>, p<2, 2>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<2, 2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<2, 2>, p<1, 1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<9, 1>, p<2, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 9>, p<2, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<9, 2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<2, 9>))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>, p<2, 2>),
                make<Map>(p<1, 1>, p<2, 2>, p<3, 3>))
            ));
        }
    }

    // Searchable
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(), equal.to(key<1>)))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>), equal.to(key<1>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(p<1, 1>), equal.to(key<2>)))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<1>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<2>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<3>)))
            );
        }

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(make<Map>(), equal.to(key<1>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(make<Map>(p<1, 1>), equal.to(key<1>)),
                just(val<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(make<Map>(p<1, 1>), equal.to(key<2>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<1>)),
                just(val<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<2>)),
                just(val<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(make<Map>(p<1, 1>, p<2, 2>), equal.to(key<3>)),
                nothing
            ));
        }
    }
}
