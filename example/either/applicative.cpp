/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/foreign.hpp>

#include <string>
using namespace boost::hana;


template <char op>
auto function = left(op + std::string{" is not a valid operation"});

template <>
auto function<'+'> = right([](auto x, auto y) {
    return x + y;
});

template <>
auto function<'-'> = right([](auto x, auto y) {
    return x - y;
});

// and so on...

template <char n>
auto digit = if_(bool_<(n >= '0' && n <= '9')>,
    right(static_cast<int>(n - 48)),
    left(n + std::string{" is not a valid digit"})
);

template <char x, char op, char y>
auto evaluate = ap(function<op>, digit<x>, digit<y>);

int main() {
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '+', '2'> == right(1 + 2));

    BOOST_HANA_RUNTIME_CHECK(evaluate<'?', '+', '2'> == left("? is not a valid digit"));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '?', '2'> == left("? is not a valid operation"));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'1', '+', '?'> == left("? is not a valid digit"));
    BOOST_HANA_RUNTIME_CHECK(evaluate<'?', '?', '?'> == left("? is not a valid operation"));
}
