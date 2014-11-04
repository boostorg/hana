/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/foreign.hpp>

#include <string>
using namespace boost::hana;


// Provided via a template parameter.
template <typename = operators::enable_adl>
struct Person_ {
    std::string name;

    struct hana { struct enabled_operators : Comparable { }; };
};
using Person = Person_<>;

// Provided via inheritance. Unfortunately, we lose PODness even though
// `operators::enable_adl` is empty.
struct Employee : operators::enable_adl {
    explicit Employee(std::string n) : name{n} { }
    std::string name;

    struct hana { struct enabled_operators : Comparable { }; };
};

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<Person, Person> : Comparable::equal_mcd {
        static bool equal_impl(Person x, Person y) {
            return x.name == y.name;
        }
    };

    template <>
    struct Comparable::instance<Employee, Employee> : Comparable::equal_mcd {
        static bool equal_impl(Employee x, Employee y) {
            return x.name == y.name;
        }
    };
}}

int main() {
    Person john{"John"}, bill{"Bill"};
    BOOST_HANA_RUNTIME_ASSERT(john == john && john != bill);

    Employee bob{"Bob"}, alice{"Alice"};
    BOOST_HANA_RUNTIME_ASSERT(bob != alice);
}
