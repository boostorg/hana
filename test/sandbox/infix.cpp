/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
#if 1
    template <char op, typename F>
    struct Left { F f; };

    template <char op, typename F>
    struct Right { F f; };

#define BOOST_HANA_DEFINE_INFIXABLE(OP, OP_CHAR)                            \
    template <typename X, typename F>                                       \
    constexpr auto operator OP(X x, Left<OP_CHAR, F> l)                     \
    { return l.f(x); }                                                      \
                                                                            \
    template <typename Y, typename F>                                       \
    constexpr auto operator OP(Right<OP_CHAR, F> r, Y y)                    \
    { return r.f(y); }                                                      \
/**/

    // Arithmetic
    BOOST_HANA_DEFINE_INFIXABLE(+, '+')
    BOOST_HANA_DEFINE_INFIXABLE(-, '-')
    BOOST_HANA_DEFINE_INFIXABLE(*, '*')
    BOOST_HANA_DEFINE_INFIXABLE(/, '/')
    BOOST_HANA_DEFINE_INFIXABLE(%, '%')
    BOOST_HANA_DEFINE_INFIXABLE(&, '&')
    BOOST_HANA_DEFINE_INFIXABLE(|, '|')
    BOOST_HANA_DEFINE_INFIXABLE(^, '^')
    // BOOST_HANA_DEFINE_INFIXABLE(<<, '<<')
    // BOOST_HANA_DEFINE_INFIXABLE(>>, '>>')

    // Comparison
    // BOOST_HANA_DEFINE_INFIXABLE(==, '==')
    // BOOST_HANA_DEFINE_INFIXABLE(!=, '!=')
    BOOST_HANA_DEFINE_INFIXABLE(<, '<')
    // BOOST_HANA_DEFINE_INFIXABLE(<=, '<=')
    BOOST_HANA_DEFINE_INFIXABLE(>, '>')
    // BOOST_HANA_DEFINE_INFIXABLE(>=, '>=')

    // Logical
    // BOOST_HANA_DEFINE_INFIXABLE(||, '||')
    // BOOST_HANA_DEFINE_INFIXABLE(&&, '&&')

#undef BOOST_HANA_DEFINE_INFIXABLE

    template <typename ...X>
    struct _inherit : X... {
        explicit constexpr _inherit(X ...x) : X(x)... { }
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto inherit = [](auto ...x) {
        return _inherit<decltype(x)...>{x...};
    };

    template <char op>
    struct _left {
        template <typename F>
        constexpr auto operator()(F f) const
        { return Left<op, decltype(f)>{f}; }
    };

    template <char op>
    struct _right {
        template <typename F>
        constexpr auto operator()(F f) const
        { return Right<op, decltype(f)>{f}; }
    };

    template <char op>
    constexpr _left<op> left{};

    template <char op>
    constexpr _right<op> right{};

    template <char left_op, char right_op>
    struct _infix {
        template <typename F>
        constexpr auto operator()(F f) const {
            auto left_to_right = left<left_op>([=](auto x) {
                return right<right_op>([=](auto y) {
                    return f(x, y);
                });
            });

            auto right_to_left = right<right_op>([=](auto y) {
                return left<left_op>([=](auto x) {
                    return f(x, y);
                });
            });

            return inherit(left_to_right, right_to_left);
        }
    };

    template <char left_op, char right_op = left_op>
    constexpr _infix<left_op, right_op> infix{};

#else
//////////////////////////////////////////////////////////////////////////////

    template <char left, char right, typename F>
    struct Infix {
        F f;
        template <typename ...X>
        constexpr auto operator()(X ...x) const
        { return f(x...); }
    };

    template <char left, char right, typename F>
    struct PartialInfix {
        F f;
        template <typename ...X>
        constexpr auto operator()(X ...x) const
        { return f(x...); }
    };

#define BOOST_HANA_DEFINE_INFIX_OPERATOR(LEFT_OP, RIGHT_OP, LEFT, RIGHT)    \
    template <typename X, typename F>                                       \
    constexpr auto operator LEFT_OP(X x, Infix<LEFT, RIGHT, F> f) {         \
        auto g = [=](auto y) { return f(x, y); };                           \
        return PartialInfix<LEFT, RIGHT, decltype(g)>{g};                   \
    }                                                                       \
                                                                            \
    template <typename X, typename F>                                       \
    constexpr auto operator LEFT_OP(X x, PartialInfix<LEFT, RIGHT, F> f)    \
    { return f(x); }                                                        \
                                                                            \
    template <typename F, typename Y>                                       \
    constexpr auto operator RIGHT_OP(Infix<LEFT, RIGHT, F> f, Y y) {        \
        auto g = [=](auto x) { return f(x, y); };                           \
        return PartialInfix<LEFT, RIGHT, decltype(g)>{g};                   \
    }                                                                       \
                                                                            \
    template <typename F, typename Y>                                       \
    constexpr auto operator RIGHT_OP(PartialInfix<LEFT, RIGHT, F> f, Y y)   \
    { return f(y); }                                                        \
/**/

    BOOST_HANA_DEFINE_INFIX_OPERATOR(*, *, '*', '*');
    BOOST_HANA_DEFINE_INFIX_OPERATOR(/, /, '/', '/');
    BOOST_HANA_DEFINE_INFIX_OPERATOR(%, %, '%', '%');

    BOOST_HANA_DEFINE_INFIX_OPERATOR(+, +, '+', '+');
    BOOST_HANA_DEFINE_INFIX_OPERATOR(-, -, '-', '-');

    BOOST_HANA_DEFINE_INFIX_OPERATOR(<, >, '<', '>');
    BOOST_HANA_DEFINE_INFIX_OPERATOR(>, <, '>', '<');

    BOOST_HANA_DEFINE_INFIX_OPERATOR(^, ^, '^', '^');

    BOOST_HANA_DEFINE_INFIX_OPERATOR(|, |, '|', '|');

#undef BOOST_HANA_DEFINE_INFIX_OPERATOR

    template <char left, char right = left>
    BOOST_HANA_CONSTEXPR_LAMBDA auto infix = [](auto f) {
        return Infix<left, right, decltype(f)>{f};
    };
#endif
}} // end namespace boost::hana

//////////////////////////////////////////////////////////////////////////////

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto plus = infix<'/'>(_ + _);
BOOST_HANA_CONSTEXPR_LAMBDA auto o = infix<'/'>(compose);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(1 /plus/ 2 == 3);

    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x % 3; };
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return x * 2; };
    BOOST_HANA_CONSTEXPR_LAMBDA auto h = [](auto x) { return x + 1; };
    BOOST_HANA_CONSTEXPR_ASSERT((f /o/ g /o/ h)(3) == ((3 + 1) * 2) % 3);
    // It's better than:
    BOOST_HANA_CONSTEXPR_ASSERT(compose(f, compose(g, h))(3) == ((3 + 1) * 2) % 3);
    // but it's not better than:
    BOOST_HANA_CONSTEXPR_ASSERT(compose(f, g, h)(3) == ((3 + 1) * 2) % 3);
}
