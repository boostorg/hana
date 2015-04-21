/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/struct_macros.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/struct.hpp>

#include <laws/base.hpp>
using namespace boost::hana;


using test::ct_eq;

namespace ns {
    struct Data0 { };
    struct Data1 {
        ct_eq<1> member1;
    };
    struct Data2 {
        ct_eq<1> member1;
        ct_eq<2> member2;
    };
    struct Data3 {
        ct_eq<1> member1;
        ct_eq<2> member2;
        ct_eq<3> member3;
    };
}

// This allows us to make sure we can enter template types
// containing commas in the macro.
template <typename T, typename ...>
using commas = T;

BOOST_HANA_ADAPT_STRUCT(ns::Data0);
BOOST_HANA_ADAPT_STRUCT(ns::Data1,
    (commas<ct_eq<1>, void>, member1)
);
BOOST_HANA_ADAPT_STRUCT(ns::Data2,
    (commas<ct_eq<1>, void, void>, member1),
    (ct_eq<2>, member2)
);
BOOST_HANA_ADAPT_STRUCT(ns::Data3,
    (ct_eq<1>, member1),
    (ct_eq<2>, member2),
    (commas<ct_eq<3>, void, void, void, void>, member3)
);

static_assert(_models<Struct, ns::Data0>{}, "");
static_assert(_models<Struct, ns::Data1>{}, "");
static_assert(_models<Struct, ns::Data2>{}, "");
static_assert(_models<Struct, ns::Data3>{}, "");

int main() {
    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("member1") ^in^ ns::Data1{});

    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("member1") ^in^ ns::Data2{});
    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("member2") ^in^ ns::Data2{});

    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("member1") ^in^ ns::Data3{});
    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("member2") ^in^ ns::Data3{});
    BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("member3") ^in^ ns::Data3{});
}
