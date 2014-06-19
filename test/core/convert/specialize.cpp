/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


struct Datatype1;
struct datatype1 {
    int value;
    using hana_datatype = Datatype1;
};

struct Datatype2;
struct datatype2 {
    int value;
    using hana_datatype = Datatype2;
};

namespace boost { namespace hana {
    template <>
    struct convert<Datatype2, Datatype1> {
        template <typename T>
        static constexpr auto apply(T t)
        { return datatype2{t.value}; }
    };
}}

int main() {
    constexpr datatype1 d1{12345};
    BOOST_HANA_CONSTEXPR_LAMBDA datatype2 d2 = to<Datatype2>(d1);
    BOOST_HANA_STATIC_ASSERT(d2.value == d1.value);

    BOOST_HANA_CONSTEXPR_LAMBDA datatype1 copy1 = to<Datatype1>(d1);
    BOOST_HANA_STATIC_ASSERT(copy1.value == d1.value);

    BOOST_HANA_CONSTEXPR_LAMBDA datatype2 copy2 = to<Datatype2>(d2);
    BOOST_HANA_STATIC_ASSERT(copy2.value == d2.value);
}
