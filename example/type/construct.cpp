/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


//! [main]
struct Person {
    std::string name;
    unsigned int age;
};

bool operator==(Person x, Person y)
{ return x.name == y.name && x.age == y.age; }

int main() {
    Person john{"John Doe", 30};
    BOOST_HANA_RUNTIME_ASSERT(type<Person>("John Doe", 30u) == john);
}
//! [main]
