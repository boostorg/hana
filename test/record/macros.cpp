/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define BOOST_PP_VARIADICS 1
#include <boost/hana/foldable/record_mcd.hpp>
#include <boost/hana/record/macros.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


namespace ns {
    using Member1 = decltype(test::x<7>);
    using Member2 = decltype(test::x<8>);

    struct Intrusive {
        BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Intrusive,
            (test::x<1>, (Member1, member1)),
            (test::x<2>, (Member2, member2))
        );
    };

    struct AdHoc {
        Member1 member1;
        Member2 member2;
    };

    // Make sure BOOST_HANA_DEFINE_RECORD_INTRUSIVE can be used inside local
    // functions.
    void intrusive_in_local_function() {
        struct Local {
            BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Local,
                (test::x<1>, (Member1, member1))
            );
        };
    }
}

BOOST_HANA_DEFINE_RECORD(ns::AdHoc,
    (test::x<1>, (ns::Member1, member1)),
    (test::x<2>, (ns::Member2, member2))
);


int main() {
    // Intrusive
    {
        ns::Intrusive intrusive{};
        (void)intrusive.member1;
        (void)intrusive.member2;

        BOOST_HANA_CONSTANT_ASSERT(is_a<Record, ns::Intrusive>);
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Tuple>(intrusive),
            tuple(intrusive.member1, intrusive.member2)
        ));
    }

    // AdHoc
    {
        ns::AdHoc adhoc{};
        BOOST_HANA_CONSTANT_ASSERT(is_a<Record, ns::AdHoc>);
        BOOST_HANA_CONSTANT_ASSERT(equal(
            to<Tuple>(adhoc),
            tuple(adhoc.member1, adhoc.member2)
        ));
    }
}
