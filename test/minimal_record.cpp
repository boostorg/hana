/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/minimal_record.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/searchable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<MinimalRecord> = make<Tuple>(
        type<Comparable>,
        type<Foldable>,
        type<Searchable>
    );

    template <>
    auto objects<MinimalRecord> = make<Tuple>(
        minimal_record(x<0>, x<0>),
        minimal_record(x<0>, x<1>),
        minimal_record(x<1>, x<0>),
        minimal_record(x<1>, x<1>),
        minimal_record(x<0>, x<2>),
        minimal_record(x<2>, x<3>)
    );
}}}

template <int i>
struct _undefined { };

template <int i = 0>
constexpr _undefined<i> undefined{};

int main() {
    using test::x;
    constexpr auto record = test::minimal_record;

    test::check_datatype<test::MinimalRecord>();

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                record(x<0>, x<1>), record(x<0>, x<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                record(x<1>, x<0>), record(x<0>, x<1>)
            )));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                record(x<0>, x<99>), record(x<0>, x<1>)
            )));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                record(x<99>, x<1>), record(x<0>, x<1>)
            )));
        }
    }

    // Foldable
    {
        auto s = x<999>;
        auto f = test::injection([]{});

        // foldl
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                foldl(record(x<0>, x<1>), s, f),
                f(f(s, x<0>), x<1>)
            ));
        }

        // foldr
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                foldr(record(x<0>, x<1>), s, f),
                f(x<0>, f(x<1>, s))
            ));
        }

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(record(x<0>, x<1>), f),
                f(x<0>, x<1>)
            ));
        }
    }

    // Searchable
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                any_of(record(undefined<1>, undefined<2>), equal.to(test::member1))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(record(undefined<1>, undefined<2>), equal.to(test::member2))
            );
            BOOST_HANA_CONSTANT_CHECK(not_(
                any_of(record(undefined<1>, undefined<2>), equal.to(undefined<>))
            ));
        }

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(record(x<0>, x<1>), equal.to(test::member1)),
                just(x<0>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(record(x<0>, x<1>), equal.to(test::member2)),
                just(x<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(record(x<0>, x<1>), equal.to(undefined<>)),
                nothing
            ));
        }
    }
}
