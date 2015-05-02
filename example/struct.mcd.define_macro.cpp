/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/struct.hpp>
#include <boost/hana/struct_macros.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


//! [main]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (int, age)
    );
};

// The member names are hana::Strings:
auto names = transform(accessors<Person>(), first);
BOOST_HANA_CONSTANT_CHECK(
    names == make_tuple(BOOST_HANA_STRING("name"), BOOST_HANA_STRING("age"))
);

int main() {
    Person john{"John", 30}, bob{"Bob", 40};
    BOOST_HANA_RUNTIME_CHECK(equal(john, john));
    BOOST_HANA_RUNTIME_CHECK(not_equal(john, bob));

    BOOST_HANA_RUNTIME_CHECK(find(john, BOOST_HANA_STRING("name")) == just("John"));
    BOOST_HANA_RUNTIME_CHECK(find(john, BOOST_HANA_STRING("age")) == just(30));
    BOOST_HANA_CONSTANT_CHECK(find(john, BOOST_HANA_STRING("foo")) == nothing);

    BOOST_HANA_RUNTIME_CHECK(to<Tuple>(john) == make<Tuple>(
        make<Pair>(BOOST_HANA_STRING("name"), "John"),
        make<Pair>(BOOST_HANA_STRING("age"), 30)
    ));

    BOOST_HANA_RUNTIME_CHECK(to<Map>(john) == make<Map>(
        make<Pair>(BOOST_HANA_STRING("name"), "John"),
        make<Pair>(BOOST_HANA_STRING("age"), 30)
    ));
}
//! [main]
