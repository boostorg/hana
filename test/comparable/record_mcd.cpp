/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/record/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/laws/comparable.hpp>
#include <test/injection.hpp>
#include <test/minimal_record.hpp>
using namespace boost::hana;


template <typename Mcd>
void tests() {
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto record = test::minimal_record<Mcd>;

    // equal
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            record(x<0>, x<1>), record(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            record(x<1>, x<0>), record(x<0>, x<1>)
        )));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            record(x<0>, x<99>), record(x<0>, x<1>)
        )));

        BOOST_HANA_CONSTANT_ASSERT(not_(equal(
            record(x<99>, x<1>), record(x<0>, x<1>)
        )));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Comparable_laws(
            record(x<0>, x<1>),
            record(x<1>, x<0>),
            record(x<0>, x<2>),
            record(x<2>, x<3>)
        ));
    }
}

int main() {
    tests<Record::mcd>();
}
