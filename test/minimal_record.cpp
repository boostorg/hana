/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/minimal_record.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/searchable.hpp>
using namespace boost::hana;


template <int i = 0>
struct undefined { };

using test::ct_eq;

int main() {
    constexpr auto record = test::minimal_record;

    auto eqs = make<Tuple>(
        record(ct_eq<0>{}, ct_eq<0>{}),
        record(ct_eq<0>{}, ct_eq<1>{}),
        record(ct_eq<1>{}, ct_eq<0>{}),
        record(ct_eq<1>{}, ct_eq<1>{}),
        record(ct_eq<0>{}, ct_eq<2>{}),
        record(ct_eq<2>{}, ct_eq<3>{})
    );
    (void)eqs;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                record(ct_eq<0>{}, ct_eq<1>{}),
                record(ct_eq<0>{}, ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                record(ct_eq<1>{}, ct_eq<0>{}),
                record(ct_eq<0>{}, ct_eq<1>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                record(ct_eq<0>{}, ct_eq<99>{}),
                record(ct_eq<0>{}, ct_eq<1>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                record(ct_eq<99>{}, ct_eq<1>{}),
                record(ct_eq<0>{}, ct_eq<1>{})
            )));
        }

        // laws
        test::TestComparable<test::MinimalRecord>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::ct_eq<999> s{};
        test::_injection<0> f{};

        // foldl
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                foldl(record(ct_eq<0>{}, ct_eq<1>{}), s, f),
                f(f(s, ct_eq<0>{}), ct_eq<1>{})
            ));
        }

        // foldr
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                foldr(record(ct_eq<0>{}, ct_eq<1>{}), s, f),
                f(ct_eq<0>{}, f(ct_eq<1>{}, s))
            ));
        }

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(record(ct_eq<0>{}, ct_eq<1>{}), f),
                f(ct_eq<0>{}, ct_eq<1>{})
            ));
        }

        // laws
        test::TestFoldable<test::MinimalRecord>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                any_of(record(undefined<1>{}, undefined<2>{}), equal.to(test::member1))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(record(undefined<1>{}, undefined<2>{}), equal.to(test::member2))
            );
            BOOST_HANA_CONSTANT_CHECK(not_(
                any_of(record(undefined<1>{}, undefined<2>{}), equal.to(undefined<>{}))
            ));
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(record(ct_eq<0>{}, ct_eq<1>{}), equal.to(test::member1)),
                just(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(record(ct_eq<0>{}, ct_eq<1>{}), equal.to(test::member2)),
                just(ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(record(ct_eq<0>{}, ct_eq<1>{}), equal.to(undefined<>{})),
                nothing
            ));
        }

        // laws
        test::TestSearchable<test::MinimalRecord>{eqs, make<Tuple>(
            test::member1, test::member2
        )};
    }
#endif
}
