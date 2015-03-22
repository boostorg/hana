/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <test/minimal_product.hpp>
#include <test/minimal_record.hpp>
#include <test/seq.hpp>
using namespace boost::hana;


template <int i>
auto key = test::ct_eq<i>{};

template <int i>
auto val = test::ct_eq<-i>{};

template <int i, int j>
auto p = test::minimal_product(key<i>, val<j>);


int main() {
    //////////////////////////////////////////////////////////////////////////
    // Map interface
    //////////////////////////////////////////////////////////////////////////
    {
        auto list = test::seq;
        auto foldable = test::seq;
        auto record = test::minimal_record;
        using L = test::Seq;

        // Make sure we do not instantiate rogue constructors when doing copies
        {
            auto expr = make<Map>(
                test::minimal_product(1, test::trap_construct{})
            );
            auto implicit_copy = expr;          (void)implicit_copy;
            decltype(expr) explicit_copy(expr); (void)explicit_copy;
        }

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
                    to<Map>(record(test::ct_eq<1>{}, test::ct_eq<2>{})),
                    make<Map>(
                        make<Pair>(test::member1, test::ct_eq<1>{}),
                        make<Pair>(test::member2, test::ct_eq<2>{})
                    )
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
}
