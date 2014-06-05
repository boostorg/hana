/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    template <char c>
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator_ = [](auto ...) {
        static_assert(c == '+', // <-- always false
        "invalid usage of operator_ with an unknown operator");
    };

#define BOOST_HANA_DEFINE_BINARY_OPERATOR(OP, OP_CHAR)                      \
    template <>                                                             \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator_<OP_CHAR> = [](auto x, auto y)\
    { return x OP y; };                                                     \
/**/

#define BOOST_HANA_DEFINE_PREFIX_OPERATOR(OP, OP_CHAR)                      \
    template <>                                                             \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator_<OP_CHAR> = [](auto x)        \
    { return OP x; };                                                       \
/**/

    // Arithmetic
    // Problem:
    // Some operators have unary and binary versions; those will clash.
    // BOOST_HANA_DEFINE_PREFIX_OPERATOR(+, '+')
    // BOOST_HANA_DEFINE_PREFIX_OPERATOR(-, '-')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(+, '+')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(-, '-')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(*, '*')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(/, '/')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(%, '%')
    BOOST_HANA_DEFINE_PREFIX_OPERATOR(~, '~')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(&, '&')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(|, '|')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(^, '^')
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(<<, '<<')
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(>>, '>>')

    // Comparison
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(==, '==')
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(!=, '!=')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(<, '<')
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(<=, '<=')
    BOOST_HANA_DEFINE_BINARY_OPERATOR(>, '>')
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(>=, '>=')

    // Logical
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(||, '||')
    // BOOST_HANA_DEFINE_BINARY_OPERATOR(&&, '&&')
    BOOST_HANA_DEFINE_PREFIX_OPERATOR(!, '!')

    // Member access
    // BOOST_HANA_DEFINE_PREFIX_OPERATOR(*, '*')

#undef BOOST_HANA_DEFINE_PREFIX_OPERATOR
#undef BOOST_HANA_DEFINE_BINARY_OPERATOR
}} // end namespace boost::hana

//////////////////////////////////////////////////////////////////////////////

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(operator_<'+'>(1, 2) == 3);
    BOOST_HANA_STATIC_ASSERT(operator_<'!'>(true) == false);
}
