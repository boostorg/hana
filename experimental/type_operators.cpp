/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
namespace hana = boost::hana;


#define BOOST_HANA_TYPE_UNARY_PREFIX_OPERATOR(OP)                           \
    template <typename T, typename = std::enable_if_t<                      \
        hana::is_a<hana::type_tag, T>()                                     \
    >>                                                                      \
    constexpr auto operator OP (T) {                                        \
        return hana::type_c<decltype(                                       \
            OP std::declval<typename T::type>()                             \
        )>;                                                                 \
    }                                                                       \
/**/

#define BOOST_HANA_TYPE_UNARY_POSTFIX_OPERATOR(OP)                          \
    template <typename T, typename = std::enable_if_t<                      \
        hana::is_a<hana::type_tag, T>()                                     \
    >>                                                                      \
    constexpr auto operator OP (T, int) {                                   \
        return hana::type_c<decltype(                                       \
            std::declval<typename T::type>() OP                             \
        )>;                                                                 \
    }                                                                       \
/**/

#define BOOST_HANA_TYPE_BINARY_OPERATOR(OP)                                 \
    template <typename T, typename U, typename = std::enable_if_t<          \
        hana::is_a<hana::type_tag, T>() && hana::is_a<hana::type_tag, U>()  \
    >>                                                                      \
    constexpr auto operator OP (T, U) {                                     \
        return hana::type_c<decltype(                                       \
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
    auto deref3 = hana::compose(deref, deref, deref);

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        *hana::type_c<char*>,
        hana::type_c<char&>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        deref(hana::type_c<int*>),
        hana::type_c<int&>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        deref3(hana::type_c<int***>),
        hana::type_c<int&>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::type_c<int> + hana::type_c<float>,
        hana::type_c<float>
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        ++hana::type_c<int&>,
        hana::type_c<int&>
    ));
}
