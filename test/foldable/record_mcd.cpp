/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/record.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
#include <test/minimal_record.hpp>
using namespace boost::hana;


template <typename Mcd>
void tests() {
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto record = test::minimal_record<Mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto s = x<999>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});

    // foldl
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            foldl(record(x<0>, x<1>), s, f),
            f(f(s, x<0>), x<1>)
        ));
    }

    // foldr
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            foldr(record(x<0>, x<1>), s, f),
            f(x<0>, f(x<1>, s))
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            record(x<0>, x<1>),
            record(x<1>, x<0>),
            record(x<2>, x<3>)
        ));
    }
}

int main() {
    tests<Record::mcd>();
}
