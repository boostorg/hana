/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional/placeholder.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
using namespace boost::hana;


struct _extra { virtual ~_extra() { } };
_extra extra{};

#define BOOST_HANA_TEST_BINARY_OP(op, x, y)                                 \
    {BOOST_HANA_CONSTEXPR_CHECK((_ op _)(x, y) == (x op y));}              \
    {BOOST_HANA_RUNTIME_CHECK((_ op _)(x, y, extra) == (x op y));}         \
    {BOOST_HANA_RUNTIME_CHECK((_ op _)(x, y, extra, extra) == (x op y));}  \
                                                                            \
    {BOOST_HANA_CONSTEXPR_CHECK((_ op y)(x) == (x op y));}                 \
    {BOOST_HANA_RUNTIME_CHECK((_ op y)(x, extra) == (x op y));}            \
    {BOOST_HANA_RUNTIME_CHECK((_ op y)(x, extra, extra) == (x op y));}     \
                                                                            \
    {BOOST_HANA_CONSTEXPR_CHECK((x op _)(y) == (x op y));}                 \
    {BOOST_HANA_RUNTIME_CHECK((x op _)(y, extra) == (x op y));}            \
    {BOOST_HANA_RUNTIME_CHECK((x op _)(y, extra, extra) == (x op y));}     \
/**/

#define BOOST_HANA_TEST_UNARY_OP(op, x)                                     \
    {BOOST_HANA_CONSTEXPR_CHECK((op _)(x) == (op x));}                     \
    {BOOST_HANA_RUNTIME_CHECK((op _)(x, extra) == (op x));}                \
    {BOOST_HANA_RUNTIME_CHECK((op _)(x, extra, extra) == (op x));}         \
/**/

int main() {
    // Arithmetic
    BOOST_HANA_TEST_UNARY_OP(+, 1)
    BOOST_HANA_TEST_UNARY_OP(-, 1)
    BOOST_HANA_TEST_BINARY_OP(+, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(-, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(*, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(/, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(%, 6, 3)

    // Bitwise
    BOOST_HANA_TEST_UNARY_OP(~, 5)
    BOOST_HANA_TEST_BINARY_OP(&, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(|, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(^, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(<<, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(>>, 6, 3)

    // Comparison
    BOOST_HANA_TEST_BINARY_OP(==, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(!=, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(<, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(<=, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(>, 6, 3)
    BOOST_HANA_TEST_BINARY_OP(>=, 6, 3)

    // Logical
    BOOST_HANA_TEST_BINARY_OP(||, true, false)
    BOOST_HANA_TEST_BINARY_OP(&&, true, true)
    BOOST_HANA_TEST_UNARY_OP(!, true)

    // Member access
    constexpr int i = 4;
    constexpr int array[] = {0, 1, 2};
    BOOST_HANA_TEST_UNARY_OP(*, &i)

    BOOST_HANA_CONSTEXPR_CHECK(_[0](array) == array[0]);
    BOOST_HANA_RUNTIME_CHECK(_[0](array, extra) == array[0]);
    BOOST_HANA_RUNTIME_CHECK(_[0](array, extra, extra) == array[0]);
    BOOST_HANA_CONSTEXPR_CHECK(_[1](array) == array[1]);
    BOOST_HANA_CONSTEXPR_CHECK(_[1](array) == array[1]);
    BOOST_HANA_CONSTEXPR_CHECK(_[2](array) == array[2]);

    // Other
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x + 1; };
    BOOST_HANA_CONSTEXPR_CHECK(_(1)(f) == f(1));
    BOOST_HANA_RUNTIME_CHECK(_(1)(f, extra) == f(1));
    BOOST_HANA_RUNTIME_CHECK(_(1)(f, extra, extra) == f(1));
    BOOST_HANA_CONSTEXPR_CHECK(_(2)(f) == f(2));
    BOOST_HANA_CONSTEXPR_CHECK(_(3)(f) == f(3));
}
