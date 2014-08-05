/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//////////////////////////////////////////////////////////////////////////////
// Important: Keep this file in sync with the Overview in the README
//////////////////////////////////////////////////////////////////////////////

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <string>
#include <tuple>
#include <type_traits>
using namespace boost::hana;


struct President { std::string name; };
struct Car       { std::string name; };
struct City      { std::string name; };
auto name = [](auto x) { return x.name; };

int main() {
    // Heterogeneous sequences for value-level metaprogramming.
    auto stuff = list(President{"Obama"}, Car{"Toyota"}, City{"Quebec"});
    assert(reverse(fmap(name, stuff)) == std::make_tuple("Quebec", "Toyota", "Obama"));

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

    // No compile-time information is lost (the assertion is done at compile-time).
    BOOST_HANA_CONSTANT_ASSERT(length(permutations(stuff)) == int_<3 * 2 * 1>);
}
