/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;
using namespace std::literals;


//! [main]
auto arithmetic_operators = make_map(
    make_pair(char_<'+'>, [](auto x, auto y) {
        return x + y;
    }),

    make_pair(char_<'-'>, [](auto x, auto y) {
        return x - y;
    })

    // and so on...
);

template <char op>
auto function() {
    return eval_if(arithmetic_operators ^contains^ char_<op>,
        [](auto _) { return right(_(arithmetic_operators)[char_<op>]); },
        [](auto _) { return left(_(op) + " is not a valid operation"s); }
    );
}

template <char n>
auto digit() {
    return if_(bool_<(n >= '0' && n <= '9')>,
        right(static_cast<int>(n - 48)),
        left(n + " is not a valid digit"s)
    );
}

template <char x, char op, char y>
auto evaluate = ap(function<op>(), digit<x>(), digit<y>());

int main() {
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '+', '2'> == right(1 + 2));

    BOOST_HANA_RUNTIME_CHECK(evaluate<'?', '+', '2'> == left("? is not a valid digit"s));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '?', '2'> == left("? is not a valid operation"s));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '+', '?'> == left("? is not a valid digit"s));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'?', '?', '?'> == left("? is not a valid operation"s));
}
//! [main]
