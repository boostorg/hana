/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair/instance.hpp>
#include <boost/hana/record/mcd.hpp>

#include <cassert>
#include <string>
using namespace boost::hana;


struct Person {
    std::string name;
    int age;
};

// The keys can be anything as long as they are compile-time comparable.
constexpr auto name = constant<decltype(&Person::name), &Person::name>;
constexpr auto age = constant<decltype(&Person::age), &Person::age>;

namespace boost { namespace hana {
    template <>
    struct Record::instance<Person> : Record::mcd {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto members_impl() {
            return list(
                pair(name, [](auto p) { return p.name; }),
                pair(age, [](auto p) { return p.age; })
            );
        }
    };
}}

int main() {
    Person john{"John", 30}, bob{"Bob", 40};
    assert(equal(john, john));
    assert(not_equal(john, bob));

    assert(lookup(name, john) == just("John"));
    assert(lookup(age, john) == just(30));
    assert(lookup(constant<int, 9>, john) == nothing);

    assert(to<List>(john) == list("John", 30));
    assert(to<Map>(john) == map(
        pair(name, "John"),
        pair(age, 30)
    ));
}
