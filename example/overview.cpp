/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//////////////////////////////////////////////////////////////////////////////
// Important: Keep this file in sync with the Overview in the README
//////////////////////////////////////////////////////////////////////////////

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


struct President { std::string name; };
struct Car       { std::string name; };
struct City      { std::string name; };
auto name = [](auto x) { return x.name; };

int main() {
    // Heterogeneous sequences for value-level metaprogramming.
    auto stuff = tuple(President{"Obama"}, Car{"Toyota"}, City{"Quebec"});
    BOOST_HANA_RUNTIME_ASSERT(reverse(fmap(name, stuff)) == tuple("Quebec", "Toyota", "Obama"));

    // No compile-time information is lost (the assertion is done at compile-time).
    BOOST_HANA_CONSTANT_ASSERT(length(stuff) == int_<3>);

    // Type-level metaprogramming works too.
    auto types = fmap(compose(metafunction<std::add_pointer>, decltype_), stuff);

    static_assert(std::is_same<
        decltype(head(types))::type, President*
    >{}, "");

    static_assert(std::is_same<
        decltype(at(int_<1>, types))::type, Car*
    >{}, "");

    static_assert(std::is_same<
        decltype(last(types))::type, City*
    >{}, "");
}
