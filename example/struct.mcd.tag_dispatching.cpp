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
};

// The keys can be anything as long as they are compile-time comparable.
constexpr auto name = integral_constant<std::string Person::*, &Person::name>;
constexpr auto age = string<'a', 'g', 'e'>;

namespace boost { namespace hana {
    template <>
    struct accessors_impl<Person> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return make_tuple(
                make_pair(name, [](auto&& p) -> decltype(auto) {
                    return id(std::forward<decltype(p)>(p).name);
                }),
                make_pair(age, [](auto&& p) -> decltype(auto) {
                    return id(std::forward<decltype(p)>(p).age);
                })
            );
        }
    };
}}
//! [main]

int main() {
    Person john{"John", 30}, bob{"Bob", 40};
    BOOST_HANA_RUNTIME_CHECK(equal(john, john));
    BOOST_HANA_RUNTIME_CHECK(not_equal(john, bob));

    BOOST_HANA_RUNTIME_CHECK(find(john, name) == just("John"));
    BOOST_HANA_RUNTIME_CHECK(find(john, age) == just(30));
    BOOST_HANA_CONSTANT_CHECK(find(john, BOOST_HANA_STRING("foo")) == nothing);

    BOOST_HANA_RUNTIME_CHECK(to<Tuple>(john) == make<Tuple>(
        make<Pair>(name, "John"),
        make<Pair>(age, 30)
    ));

    BOOST_HANA_RUNTIME_CHECK(to<Map>(john) == make<Map>(
        make<Pair>(name, "John"),
        make<Pair>(age, 30)
    ));
}
