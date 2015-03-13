/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


int main() {

{

//! [make<Map>]
using namespace std::literals;
auto m = make<Map>(
    make<Pair>(int_<1>, "foobar"s),
    make<Pair>(type<void>, 1234)
);
//! [make<Map>]
(void)m;

}{

//! [make_map]
using namespace std::literals;
BOOST_HANA_RUNTIME_CHECK(
    make_map(
        make<Pair>(int_<1>, "foobar"s),
        make<Pair>(type<void>, 1234)
    )
    ==
    make<Map>(
        make<Pair>(int_<1>, "foobar"s),
        make<Pair>(type<void>, 1234)
    )
);
//! [make_map]

}{

//! [comparable]
using namespace std::literals;
BOOST_HANA_RUNTIME_CHECK(
    make_map(
        pair(char_<'a'>, "foobar"s),
        pair(type<int&&>, nullptr)
    )
    ==
    make_map(
        pair(type<int&&>, (void*)0),
        pair(char_<'a'>, "foobar"s)
    )
);
//! [comparable]

}{

//! [searchable]
BOOST_HANA_CONSTEXPR_LAMBDA auto m = make_map(
    pair(type<int>, 'i'),
    pair(type<float>, 'f')
);
BOOST_HANA_CONSTEXPR_CHECK(find(m, type<int>) == just('i'));
BOOST_HANA_CONSTEXPR_CHECK(find(m, type<float>) == just('f'));
BOOST_HANA_CONSTANT_CHECK(find(m, type<void>) == nothing);
BOOST_HANA_CONSTANT_CHECK(find(m, int_<3>) == nothing);
//! [searchable]

}

}
