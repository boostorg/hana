/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


//! [main]
struct Person {
    std::string name;
    int age;

    struct hana { struct members_impl {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return make<Tuple>(
                make<Pair>(BOOST_HANA_STRING("name"), [](auto&& p) {
                    return p.name;
                }),
                make<Pair>(BOOST_HANA_STRING("age"), [](auto&& p) {
                    return p.age;
                })
            );
        }
    };};
};

int main() {
    Person john{"John", 35};
    BOOST_HANA_RUNTIME_ASSERT(to<Map>(john) == make<Map>(
        make<Pair>(BOOST_HANA_STRING("name"), "John"),
        make<Pair>(BOOST_HANA_STRING("age"), 35)
    ));
}
//! [main]
