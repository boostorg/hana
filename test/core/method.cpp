/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/method.hpp>
#include <boost/hana/core/when.hpp>
using namespace boost::hana;


namespace base_case {
    // Base case:
    //  a(T) implemented by b(T)
    //  b(T) implemented by a(T) or by c(T)
    //  c(mytype) implemented
    //  ------
    //  a(mytype) should use c(mytype) instead of recursing
    BOOST_HANA_METHOD(a_impl);
    BOOST_HANA_METHOD(b_impl);
    BOOST_HANA_METHOD(c_impl);

    template <typename T, typename _>
    struct a_impl<T, _, when<
        is_implemented<b_impl<T>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    template <typename T, typename _>
    struct b_impl<T, _, when<
        is_implemented<a_impl<T>, _>
    >> { };

    template <typename T, typename _>
    struct b_impl<T, _, when<
        is_implemented<c_impl<T>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    struct MyType;
    template <typename _>
    struct c_impl<MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType>::dispatches_correctly, "");
    static_assert(b_impl<MyType>::dispatches_correctly, "");
    static_assert(c_impl<MyType>::dispatches_correctly, "");

    static_assert(is_implemented<a_impl<MyType>>, "");
    static_assert(is_implemented<b_impl<MyType>>, "");
    static_assert(is_implemented<c_impl<MyType>>, "");
}

namespace base_case_binary {
    // Base case with binary methods:
    //  a(T, U) implemented by b(T, U)
    //  b(T, U) implemented by a(T, U) or by c(T, U)
    //  c(mytype, mytype) implemented
    //  ------
    //  a(mytype, mytype) should use c(mytype, mytype) instead of recursing
    BOOST_HANA_BINARY_METHOD(a_impl);
    BOOST_HANA_BINARY_METHOD(b_impl);
    BOOST_HANA_BINARY_METHOD(c_impl);

    template <typename T, typename U, typename _>
    struct a_impl<T, U, _, when<
        is_implemented<b_impl<T, U>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    template <typename T, typename U, typename _>
    struct b_impl<T, U, _, when<
        is_implemented<a_impl<T, U>, _>
    >> { };

    template <typename T, typename U, typename _>
    struct b_impl<T, U, _, when<
        is_implemented<c_impl<T, U>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    struct MyType;
    template <typename _>
    struct c_impl<MyType, MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType, MyType>::dispatches_correctly, "");
    static_assert(b_impl<MyType, MyType>::dispatches_correctly, "");
    static_assert(c_impl<MyType, MyType>::dispatches_correctly, "");

    static_assert(is_implemented<a_impl<MyType, MyType>>, "");
    static_assert(is_implemented<b_impl<MyType, MyType>>, "");
    static_assert(is_implemented<c_impl<MyType, MyType>>, "");
}

namespace degenerate_base_case {
    // Degenerate version of the base case:
    //  a(T) implemented by a(T) or b(T)
    //  b(mytype) implemented
    //  ------
    //  a(mytype) should use b(mytype) instead of recursing
    BOOST_HANA_METHOD(a_impl);
    BOOST_HANA_METHOD(b_impl);

    template <typename T, typename _>
    struct a_impl<T, _, when<
        is_implemented<a_impl<T>, _>
    >> { };

    template <typename T, typename _>
    struct a_impl<T, _, when<
        is_implemented<b_impl<T>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    struct MyType;
    template <typename _>
    struct b_impl<MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType>::dispatches_correctly, "");
    static_assert(b_impl<MyType>::dispatches_correctly, "");

    static_assert(is_implemented<a_impl<MyType>>, "");
    static_assert(is_implemented<b_impl<MyType>>, "");
}

namespace degenerate_base_case_binary {
    // Degenerate version of the base case with binary methods:
    //  a(T, U) implemented by a(T, U) or b(T, U)
    //  b(mytype, mytype) implemented
    //  ------
    //  a(mytype, mytype) should use b(mytype, mytype) instead of recursing
    BOOST_HANA_BINARY_METHOD(a_impl);
    BOOST_HANA_BINARY_METHOD(b_impl);

    template <typename T, typename U, typename _>
    struct a_impl<T, U, _, when<
        is_implemented<a_impl<T, U>, _>
    >> { };

    template <typename T, typename U, typename _>
    struct a_impl<T, U, _, when<
        is_implemented<b_impl<T, U>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    struct MyType;
    template <typename _>
    struct b_impl<MyType, MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType, MyType>::dispatches_correctly, "");
    static_assert(b_impl<MyType, MyType>::dispatches_correctly, "");
}

namespace priority_over_when {
    // Make sure that a method specialized without `when` has priority over a
    // method specialized with `when`.
    BOOST_HANA_METHOD(a_impl);

    template <typename T, typename _>
    struct a_impl<T, _, when<true>> { };

    struct MyType;
    template <typename _>
    struct a_impl<MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType>::dispatches_correctly, "");
    static_assert(is_implemented<a_impl<MyType>>, "");
}

namespace priority_over_when_with_parametric_type {
    // Make sure that a method specialized without `when` has priority over a
    // method specialized with `when` even in the case of a parametric type.
    BOOST_HANA_METHOD(a_impl);

    template <typename T, typename _>
    struct a_impl<T, _, when<true>> { };

    template <typename T> struct MyType;
    template <typename T, typename _>
    struct a_impl<MyType<T>, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType<void>>::dispatches_correctly, "");
    static_assert(is_implemented<a_impl<MyType<void>>>, "");
}

int main() { }
