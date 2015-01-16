/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <boost/hana/record.hpp>

#include <string>
#include <utility>
using namespace boost::hana;


struct Person {
    std::string name;
    int age;
};

// The keys can be anything as long as they are compile-time comparable.
constexpr auto name = decltype_(&Person::name);
constexpr auto age = decltype_(&Person::age);

namespace boost { namespace hana {
    template <>
    struct members_impl<Person> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return tuple(
                pair(name, [](auto&& p) -> decltype(auto) {
                    return id(std::forward<decltype(p)>(p).name);
                }),
                pair(age, [](auto&& p) -> decltype(auto) {
                    return id(std::forward<decltype(p)>(p).age);
                })
            );
        }
    };
}}

int main() {
    Person john{"John", 30}, bob{"Bob", 40};
    BOOST_HANA_RUNTIME_CHECK(equal(john, john));
    BOOST_HANA_RUNTIME_CHECK(not_equal(john, bob));

    BOOST_HANA_RUNTIME_CHECK(lookup(john, name) == just("John"));
    BOOST_HANA_RUNTIME_CHECK(lookup(john, age) == just(30));
    BOOST_HANA_CONSTANT_CHECK(lookup(john, "clearly not a member") == nothing);

    BOOST_HANA_RUNTIME_CHECK(to<Tuple>(john) == tuple("John", 30));
    BOOST_HANA_RUNTIME_CHECK(to<Map>(john) == map(
        pair(name, "John"),
        pair(age, 30)
    ));
}
