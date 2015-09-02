/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


#define BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(OP)                           \
    template <typename T, typename = std::enable_if_t<is_a<Type, T>>>       \
    constexpr auto operator OP (T) {                                        \
        return type_c<decltype(                                             \
            OP std::declval<typename T::type>()                             \
        )>;                                                                 \
    }                                                                       \
/**/

#define BOOST_HANA_TYPE_UNARY_POSTFIX_OPERATOR(OP)                          \
    template <typename T, typename = std::enable_if_t<is_a<Type, T>()>>     \
    constexpr auto operator OP (T, int) {                                   \
        return type_c<decltype(                                             \
            std::declval<typename T::type>() OP                             \
        )>;                                                                 \
    }                                                                       \
/**/

#define BOOST_HANA_TYPE_BINARY_OPERATOR(OP)                                 \
    template <typename T, typename U, typename = std::enable_if_t<          \
        is_a<Type, T>() && is_a<Type, U>()                                  \
    >>                                                                      \
    constexpr auto operator OP (T, U) {                                     \
        return type_c<decltype(                                             \
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
        *type_c<char*>,
        type_c<char&>
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        deref(type_c<int*>),
        type_c<int&>
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        deref3(type_c<int***>),
        type_c<int&>
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        type_c<int> + type_c<float>,
        type_c<float>
    ));
}
