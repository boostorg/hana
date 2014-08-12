/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/record/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/minimal_record.hpp>
using namespace boost::hana;


template <int i>
struct _undefined { };

template <int i = 0>
constexpr _undefined<i> undefined{};

template <typename Mcd>
void tests() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto is = [](auto k) {
        return [=](auto key) { return equal(k, key); };
    };

    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto record = test::minimal_record<Mcd>;

    // any
    {
        BOOST_HANA_CONSTANT_ASSERT(
            any(record(undefined<1>, undefined<2>), is(test::member1))
        );
        BOOST_HANA_CONSTANT_ASSERT(
            any(record(undefined<1>, undefined<2>), is(test::member2))
        );
        BOOST_HANA_CONSTANT_ASSERT(not_(
            any(record(undefined<1>, undefined<2>), is(undefined<>))
        ));
    }

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(record(x<0>, x<1>), is(test::member1)),
            just(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(record(x<0>, x<1>), is(test::member2)),
            just(x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(record(x<0>, x<1>), is(undefined<>)),
            nothing
        ));
    }
}

int main() {
    tests<Record::mcd>();
}
