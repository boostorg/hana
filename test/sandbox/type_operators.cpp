/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


#define BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(OP)                           \
    template <typename T, typename = std::enable_if_t<is_a<Type, T>>>       \
    constexpr auto operator OP (T) {                                        \
        return type<decltype(                                               \
            OP std::declval<typename T::type>()                             \
        )>;                                                                 \
    }                                                                       \
/**/

#define BOOST_HANA_TYPE_UNARY_POSTFIX_OPERATOR(OP)                          \
    template <typename T, typename = std::enable_if_t<is_a<Type, T>()>>     \
    constexpr auto operator OP (T, int) {                                   \
        return type<decltype(                                               \
            std::declval<typename T::type>() OP                             \
        )>;                                                                 \
    }                                                                       \
/**/

#define BOOST_HANA_TYPE_BINARY_OPERATOR(OP)                                 \
    template <typename T, typename U, typename = std::enable_if_t<          \
        is_a<Type, T>() && is_a<Type, U>()                                  \
    >>                                                                      \
    constexpr auto operator OP (T, U) {                                     \
        return type<decltype(                                               \
            std::declval<typename T::type>()                                \
            OP                                                              \
            std::declval<typename U::type>()                                \
        )>;                                                                 \
    }                                                                       \
/**/


BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(+)
BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(-)
BOOST_HANA_TYPE_BINARY_OPERATOR(+)
BOOST_HANA_TYPE_BINARY_OPERATOR(-)
BOOST_HANA_TYPE_BINARY_OPERATOR(*)
BOOST_HANA_TYPE_BINARY_OPERATOR(/)
BOOST_HANA_TYPE_BINARY_OPERATOR(%)

BOOST_HANA_TYPE_BINARY_OPERATOR(<)
BOOST_HANA_TYPE_BINARY_OPERATOR(>)
BOOST_HANA_TYPE_BINARY_OPERATOR(<=)
BOOST_HANA_TYPE_BINARY_OPERATOR(>=)

BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(++)
BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(--)
BOOST_HANA_TYPE_UNARY_POSTFIX_OPERATOR(++)
BOOST_HANA_TYPE_UNARY_POSTFIX_OPERATOR(--)

BOOST_HANA_TYPE_BINARY_OPERATOR(<<)
BOOST_HANA_TYPE_BINARY_OPERATOR(>>)
BOOST_HANA_TYPE_BINARY_OPERATOR(|)
BOOST_HANA_TYPE_BINARY_OPERATOR(&)
BOOST_HANA_TYPE_BINARY_OPERATOR(^)
BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(~)

BOOST_HANA_TYPE_BINARY_OPERATOR(&&)
BOOST_HANA_TYPE_BINARY_OPERATOR(||)
BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(!)

BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(*)
BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(&)


int main() {
    auto deref = [](auto x) { return *x; };
    auto deref3 = compose(deref, deref, deref);

    BOOST_HANA_CONSTANT_CHECK(equal(
        *type<char*>,
        type<char&>
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        deref(type<int*>),
        type<int&>
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        deref3(type<int***>),
        type<int&>
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        type<int> + type<float>,
        type<float>
    ));
}
