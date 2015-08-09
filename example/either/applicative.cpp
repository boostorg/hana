/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ap.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>

#include <string>
namespace hana = boost::hana;
using namespace std::literals;


auto arithmetic_operators = hana::make_map(
    hana::make_pair(hana::char_<'+'>, [](auto x, auto y) {
        return x + y;
    }),

    hana::make_pair(hana::char_<'-'>, [](auto x, auto y) {
        return x - y;
    })

    // and so on...
);

template <char op>
auto function() {
    return hana::eval_if(arithmetic_operators ^hana::contains^ hana::char_<op>,
        [](auto _) { return hana::right(_(arithmetic_operators)[hana::char_<op>]); },
        [](auto _) { return hana::left(_(op) + " is not a valid operation"s); }
    );
}

template <char n>
auto digit() {
    return hana::if_(hana::bool_<(n >= '0' && n <= '9')>,
        hana::right(static_cast<int>(n - 48)),
        hana::left(n + " is not a valid digit"s)
    );
}

template <char x, char op, char y>
auto evaluate = hana::ap(function<op>(), digit<x>(), digit<y>());

int main() {
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '+', '2'> == hana::right(1 + 2));

    BOOST_HANA_RUNTIME_CHECK(evaluate<'?', '+', '2'> == hana::left("? is not a valid digit"s));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '?', '2'> == hana::left("? is not a valid operation"s));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '+', '?'> == hana::left("? is not a valid digit"s));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'?', '?', '?'> == hana::left("? is not a valid operation"s));
}
