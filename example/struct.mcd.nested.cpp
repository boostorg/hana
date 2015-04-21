/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/struct.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
#include <utility>
using namespace boost::hana;


//! [main]
struct Person {
    std::string name;
    int age;

    struct hana { struct accessors_impl {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return boost::hana::make_tuple(
                boost::hana::make_pair(BOOST_HANA_STRING("name"),
                [](auto&& p) -> decltype(auto) {
                    return boost::hana::id(std::forward<decltype(p)>(p).name);
                }),
                boost::hana::make_pair(BOOST_HANA_STRING("age"),
                [](auto&& p) -> decltype(auto) {
                    return boost::hana::id(std::forward<decltype(p)>(p).age);
                })
            );
        }
    };};
};
//! [main]

int main() {
    Person john{"John", 30}, bob{"Bob", 40};
    BOOST_HANA_RUNTIME_CHECK(equal(john, john));
    BOOST_HANA_RUNTIME_CHECK(not_equal(john, bob));

    BOOST_HANA_RUNTIME_CHECK(find(john, BOOST_HANA_STRING("name")) == just("John"));
    BOOST_HANA_RUNTIME_CHECK(find(john, BOOST_HANA_STRING("age")) == just(30));
    BOOST_HANA_CONSTANT_CHECK(find(john, "clearly not a member") == nothing);

    BOOST_HANA_RUNTIME_CHECK(to<Tuple>(john) == make<Tuple>(
        make<Pair>(BOOST_HANA_STRING("name"), "John"),
        make<Pair>(BOOST_HANA_STRING("age"), 30)
    ));

    BOOST_HANA_RUNTIME_CHECK(to<Map>(john) == make<Map>(
        make<Pair>(BOOST_HANA_STRING("name"), "John"),
        make<Pair>(BOOST_HANA_STRING("age"), 30)
    ));
}
