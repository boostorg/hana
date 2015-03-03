/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>
#include <boost/hana/record_macros.hpp>
#include <boost/hana/searchable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


namespace ns {
    using Member1 = decltype(int_<1>);

    // Parametric so we can use multiple template arguments inside the
    // macros, just to make sure it works as expected.
    template <typename ...>
    using Member2 = decltype(int_<2>);

    struct Intrusive {
        BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Intrusive,
            (Member1, member1),
            (Member2<void, void>, member2)
        );
    };

    struct AdHoc {
        Member1 member1;
        Member2<void, void> member2;
    };

    // Make sure BOOST_HANA_DEFINE_RECORD_INTRUSIVE can be used inside local
    // functions.
    void intrusive_in_local_function() {
        struct Local {
            BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Local,
                (Member1, member1)
            );
        };
    }
}

BOOST_HANA_DEFINE_RECORD(ns::AdHoc,
    (ns::Member1, member1),
    (ns::Member2<void, void>, member2)
);


int main() {
    // Intrusive
    {
        ns::Intrusive intrusive{};
        (void)intrusive.member1;
        (void)intrusive.member2;

        BOOST_HANA_CONSTANT_CHECK(is_a<Record, ns::Intrusive>);
        BOOST_HANA_CONSTANT_CHECK(equal(
            to<Tuple>(intrusive),
            make<Tuple>(intrusive.member1, intrusive.member2)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            lookup(intrusive, BOOST_HANA_STRING("member1")),
            just(intrusive.member1)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            lookup(intrusive, BOOST_HANA_STRING("member2")),
            just(intrusive.member2)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            lookup(intrusive, BOOST_HANA_STRING("inexistant")),
            nothing
        ));
    }

    // AdHoc
    {
        ns::AdHoc adhoc{};
        BOOST_HANA_CONSTANT_CHECK(is_a<Record, ns::AdHoc>);
        BOOST_HANA_CONSTANT_CHECK(equal(
            to<Tuple>(adhoc),
            make<Tuple>(adhoc.member1, adhoc.member2)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            lookup(adhoc, BOOST_HANA_STRING("member1")),
            just(adhoc.member1)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            lookup(adhoc, BOOST_HANA_STRING("member2")),
            just(adhoc.member2)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            lookup(adhoc, BOOST_HANA_STRING("inexistant")),
            nothing
        ));
    }
}
