/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral.hpp>
using namespace boost::hana;


/*

Clang 3.5 says:
------------------------------------
test/integral/bug_constexpr_value.cpp:17:20: error: constexpr variable 'x' must be initialized by a constant expression
    constexpr auto x = value(t);
                   ^   ~~~~~~~~
test/integral/bug_constexpr_value.cpp:22:5: note: in instantiation of function template specialization 'function<boost::hana::integral_detail::integral_type<int, 1> >' requested here
    function(int_<1>);
    ^
test/integral/bug_constexpr_value.cpp:17:30: note: read of non-constexpr variable 't' is not allowed in a constant expression
    constexpr auto x = value(t);
                             ^
test/integral/bug_constexpr_value.cpp:17:30: note: in call to 'integral_type(t)'
test/integral/bug_constexpr_value.cpp:16:27: note: declared here
constexpr void function(T t) {
                          ^
------------------------------------

if the friend `value` function has signature

    constexpr T value(T)

instead of

    constexpr T value(T const&)

It seems like the problem only arises in dependent contexts.

*/

template <typename T>
constexpr void function(T t) {
    constexpr auto x = value(t);
    (void)x;
}

int main() {
    function(int_<1>);
}
