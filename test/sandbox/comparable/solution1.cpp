/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>

#include <cassert>
#include <string>
#include <type_traits>
namespace hana = boost::hana;


/*
We have a way of knowing whether a data type uses the operators provided by
a type class. Then, if any of the data types use these operators, we don't
provide a default type class instance. Otherwise, we provide a default type
class instance iff the `x op y` expression is well-formed.

We use named operators in the library.
 */
namespace hana2 { struct Comparable; }
namespace boost { namespace hana {
    BOOST_HANA_BINARY_TYPECLASS_BOILERPLATE(::hana2::Comparable)
}}

namespace mpl {
    template <typename X, typename Y, typename ...Z>
    struct or_ : or_<X, or_<Y, Z...>> { };

    template <typename X, typename Y>
    struct or_<X, Y> : std::conditional_t<X::type::value, X, Y> { };


    template <typename X, typename Y, typename ...Z>
    struct and_ : and_<X, and_<Y, Z...>> { };

    template <typename X, typename Y>
    struct and_<X, Y> : std::conditional_t<X::type::value, Y, X> { };


    template <typename T>
    struct not_ : std::integral_constant<bool, !T::type::value> { };


    template <typename T, typename U, typename = void>
    struct is_comparable : std::false_type { };

    template <typename T, typename U>
    struct is_comparable<T, U, decltype((void)(std::declval<T>() == std::declval<U>()))> : std::true_type { };
}

namespace hana2 {
    namespace operators {
        struct enable { };

        template <typename T>
        std::true_type uses_operators_impl(T, int);
    }

    template <typename T>
    std::false_type uses_operators_impl(T, long);

    template <typename T>
    using uses_operators = decltype(uses_operators_impl(*(T*)0, int{}));

    struct Comparable : hana::binary_typeclass<Comparable> { };

    template <typename X, typename Y>
    constexpr auto equal(X x, Y y) {
        return Comparable::instance<
            hana::datatype_t<X>, hana::datatype_t<Y>
        >::equal_impl(x, y);
    }
}

template <typename T, typename U>
struct hana2::Comparable::instance<T, U, std::enable_if_t<
    mpl::and_<
        mpl::not_<hana2::uses_operators<T>>,
        mpl::not_<hana2::uses_operators<U>>,
        mpl::is_comparable<T, U>
    >{}
>> {
    template <typename X, typename Y>
    static constexpr auto equal_impl(X x, Y y) {
        return x == y;
    }
};

struct UsingOperators : hana2::operators::enable { };


int main() {
    static_assert(hana2::uses_operators<UsingOperators>{}, "");
    static_assert(!hana2::uses_operators<int>{}, "");

    static_assert(hana::instantiates<hana2::Comparable, int, int>, "");
    static_assert(!hana::instantiates<hana2::Comparable, UsingOperators, UsingOperators>, "");
    static_assert(!hana::instantiates<hana2::Comparable, int, UsingOperators>, "");
    static_assert(!hana::instantiates<hana2::Comparable, UsingOperators, int>, "");
    static_assert(hana::instantiates<hana2::Comparable, std::string, char const*>, "");

    static_assert(hana2::equal(0, 0), "");
    static_assert(!hana2::equal(0, 1), "");
    assert(hana2::equal(std::string{"abcd"}, "abcd"));
    // hana2::equal(UsingOperators{}, UsingOperators{});
}
