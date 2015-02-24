/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/operators.hpp>

#include <string>
using namespace boost::hana;


// The operators are made findable to ADL by making `operators::adl` a
// template parameter.
template <typename = operators::adl>
struct Person_ { std::string name; };
using Person = Person_<>;

// Once the operators can be found by name lookup, we still need to
// specialize the `operators::of` trait to enable them, or we could
// have used a nested `hana::operators` type too.
namespace boost { namespace hana { namespace operators {
    template <>
    struct of<Person>
        : operators::of<Comparable>
    { };
}}}

// Here, the operators are made findable by ADL by using `operators::adl` as
// a base class. Unfortunately, we lose PODness even though `operators::adl`
// is empty, which is why it might sometimes be useful to use `operators::adl`
// as a template parameter instead.
struct Employee : operators::adl {
    explicit Employee(std::string n) : name{n} { }
    std::string name;

    // This time, we use the nested `hana::operators` type to enable the
    // operators for Comparable.
    struct hana {
        struct operators
            : boost::hana::operators::of<Comparable>
        { };
    };
};

namespace boost { namespace hana {
    template <>
    struct equal_impl<Person, Person> {
        static bool apply(Person x, Person y)
        { return x.name == y.name; }
    };

    template <>
    struct equal_impl<Employee, Employee> {
        static bool apply(Employee x, Employee y)
        { return x.name == y.name; }
    };
}}

int main() {
    static_assert(has_operator<Person, decltype(equal)>, "");
    static_assert(has_operator<Person, decltype(not_equal)>, "");

    static_assert(has_operator<Employee, decltype(equal)>, "");
    static_assert(has_operator<Employee, decltype(not_equal)>, "");

    Person john{"John"}, bill{"Bill"};
    BOOST_HANA_RUNTIME_CHECK(john == john && john != bill);

    Employee bob{"Bob"}, alice{"Alice"};
    BOOST_HANA_RUNTIME_CHECK(bob != alice);
}
