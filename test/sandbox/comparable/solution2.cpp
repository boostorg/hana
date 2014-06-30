/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>

#include <type_traits>
namespace hana = boost::hana;


/*
We provide an `equal` with the following definition:

    if (instantiates<Comparable, T, U>)
        return Comparable::instance<T, U>::equal_impl(x, y);
    else
        return x == y;

2. In the library, we use that `equal` everywhere; we never use `operator==`.
3. We do not provide a default `Comparable` type class, since it is handled
   by our `equal`.
4. We may provide an `operator==` which is equivalent to
   `Comparable::instance<>::equal_impl`, and which is only enabled when the
   instance exists. We put it in a separate namespace and provide a way of
   making it ADL-findable for a data type when desired.

Pros:
- We get the default operators.
- We use the instance's version of comparison when it exists even for data
  types with an `==`. So for instance, comparing `std::tuple`s of `Integral`s
  will yield a `bool_` instead of a `bool`, even if there is an `==` for
  `std::tuple`s.

Cons:
- We use names instead of cute operators in the library.
 */
namespace hana2 { struct Comparable; }
namespace boost { namespace hana {
    BOOST_HANA_BINARY_TYPECLASS_BOILERPLATE(::hana2::Comparable)
}}

namespace hana2 {
    struct Comparable : hana::binary_typeclass<Comparable> { };

    template <typename X, typename Y, typename = std::enable_if_t<
        hana::instantiates<Comparable, hana::datatype_t<X>, hana::datatype_t<Y>>
    >>
    constexpr auto equal_impl(X x, Y y, int) {
        return Comparable::instance<
            hana::datatype_t<X>, hana::datatype_t<Y>
        >::equal_impl(x, y);
    }

    template <typename X, typename Y>
    constexpr auto equal_impl(X x, Y y, long) {
        return x == y;
    }

    template <typename X, typename Y>
    constexpr auto equal(X x, Y y) {
        return equal_impl(x, y, int{});
    }
}


int main() {
    static_assert(hana2::equal(0, 0), "");
    static_assert(!hana2::equal(0, 1), "");

    static_assert(!hana::instantiates<hana2::Comparable, int, int>, "");
}
