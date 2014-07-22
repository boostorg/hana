/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

#include <boost/hana/detail/static_assert.hpp>

#include <boost/hana.hpp>
using namespace boost::hana;


int main() {
    // Value level programming
    auto xs = list(1, '2', std::string{"345"});

    assert(last(xs) == "345");
    assert(tail(xs) == list('2', std::string{"345"}));
    BOOST_HANA_STATIC_ASSERT(!is_empty(xs));
    for_each(xs, [](auto x) { std::cout << x; });

    auto to_xml = [](auto& ostream, auto xs) {
        for_each(xs, [&](auto x) {
            ostream << '<'  << typeid(x).name() << '>'
                    <<                x
                    << "</" << typeid(x).name() << '>';
        });
    };
    to_xml(std::cout, xs);


    // Type level programming
    auto ts = list(type<int*>, type<void>, type<char const>);
    BOOST_HANA_STATIC_ASSERT(last(ts) == type<char const>);
    BOOST_HANA_STATIC_ASSERT(fmap(metafunction<std::add_pointer>, ts) == list(type<int**>, type<void*>, type<char const*>));

    static_assert(std::is_same<
        decltype(last(ts))::type,
        char const
    >::value, "");
}
