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
We provide a default instance for `Comparable` which is enabled whenever
`T{} == U{}`. Since C++ types with a data type different from their C++
type should be (as a rule of thumb) those defining instances, we make an
acceptable error.
 */
namespace hana2 { struct Comparable; }
namespace boost { namespace hana {
    BOOST_HANA_BINARY_TYPECLASS_BOILERPLATE(::hana2::Comparable)
}}

namespace hana2 {
    struct Comparable : hana::binary_typeclass<Comparable> { };

    template <typename X, typename Y>
    constexpr auto equal(X x, Y y) {
        return Comparable::instance<
            hana::datatype_t<X>, hana::datatype_t<Y>
        >::equal_impl(x, y);
    }
}

template <typename T, typename U>
struct hana2::Comparable::instance<T, U, decltype((void)(*(T*)0 == *(U*)0))> {
    static constexpr auto equal_impl(T x, U y)
    { return x == y; }
};


int main() {
    static_assert(hana::instantiates<hana2::Comparable, int, int>, "");
    static_assert(hana::instantiates<hana2::Comparable, std::string, char const*>, "");
    static_assert(!hana::instantiates<hana2::Comparable, int, void>, "");
    static_assert(!hana::instantiates<hana2::Comparable, std::string, int>, "");

    static_assert(hana2::equal(0, 0), "");
    static_assert(!hana2::equal(0, 1), "");
    assert(hana2::equal(std::string{"abcd"}, "abcd"));
}
