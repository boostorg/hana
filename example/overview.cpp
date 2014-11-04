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
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


struct President { std::string name; };
struct Car       { std::string name; };
struct City      { std::string name; };

int main() {
    // Heterogeneous sequences for value-level metaprogramming.
    auto stuff = tuple(President{"Obama"}, Car{"Toyota"}, City{"Quebec"});

    auto names = fmap(stuff, [](auto thing) { return thing.name; });
    BOOST_HANA_RUNTIME_ASSERT(reverse(names) == tuple("Quebec", "Toyota", "Obama"));

    // No compile-time information is lost:
    // `stuff` wasn't constexpr but its length is!
    static_assert(length(stuff) == 3u, "");

    // Type-level metaprogramming works too.
    auto types = fmap(stuff, [](auto thing) {
        return type<decltype(thing)*>;
    });

    BOOST_HANA_CONSTANT_ASSERT(
        types == tuple(type<President*>, type<Car*>, type<City*>)
    );
}
