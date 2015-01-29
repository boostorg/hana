/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/datatype.hpp>
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
    struct a_impl<T, when<is_implemented<b_impl<T>, _>>, _> {
        static constexpr bool a_from_b = true;
    };


    template <typename T, typename _>
    struct b_impl<T, when<is_implemented<a_impl<T>, _>>, _> {
        static constexpr bool b_from_a = true;
    };

    template <typename T, typename _>
    struct b_impl<T, when<is_implemented<c_impl<T>, _>>, _> {
        static constexpr bool b_from_c = true;
    };


    struct MyType;
    template <>
    struct c_impl<MyType> {
        static constexpr bool c_MyType = true;
    };

    static_assert(dispatch<a_impl<MyType>>::a_from_b, "");
    static_assert(dispatch<b_impl<MyType>>::b_from_c, "");
    static_assert(dispatch<c_impl<MyType>>::c_MyType, "");

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
    struct a_impl<T, U, when<is_implemented<b_impl<T, U>, _>>, _> {
        static constexpr bool a_from_b = true;
    };


    template <typename T, typename U, typename _>
    struct b_impl<T, U, when<is_implemented<a_impl<T, U>, _>>, _> {
        static constexpr bool b_from_a = true;
    };

    template <typename T, typename U, typename _>
    struct b_impl<T, U, when<is_implemented<c_impl<T, U>, _>>, _> {
        static constexpr bool b_from_c = true;
    };


    struct MyType;
    template <>
    struct c_impl<MyType, MyType> {
        static constexpr bool c_MyType = true;
    };

    static_assert(dispatch<a_impl<MyType, MyType>>::a_from_b, "");
    static_assert(dispatch<b_impl<MyType, MyType>>::b_from_c, "");
    static_assert(dispatch<c_impl<MyType, MyType>>::c_MyType, "");

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
    struct a_impl<T, when<is_implemented<a_impl<T>, _>>, _> {
        static constexpr bool a_from_a = true;
    };

    template <typename T, typename _>
    struct a_impl<T, when<is_implemented<b_impl<T>, _>>, _> {
        static constexpr bool a_from_b = true;
    };


    struct MyType;
    template <>
    struct b_impl<MyType> {
        static constexpr bool b_MyType = true;
    };

    static_assert(dispatch<a_impl<MyType>>::a_from_b, "");
    static_assert(dispatch<b_impl<MyType>>::b_MyType, "");

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
    struct a_impl<T, U, when<is_implemented<a_impl<T, U>, _>>, _> {
        static constexpr bool a_from_a = true;
    };

    template <typename T, typename U, typename _>
    struct a_impl<T, U, when<is_implemented<b_impl<T, U>, _>>, _> {
        static constexpr bool a_from_b = true;
    };


    struct MyType;
    template <>
    struct b_impl<MyType, MyType> {
        static constexpr bool b_MyType = true;
    };

    static_assert(dispatch<a_impl<MyType, MyType>>::a_from_b, "");
    static_assert(dispatch<b_impl<MyType, MyType>>::b_MyType, "");

    static_assert(is_implemented<a_impl<MyType, MyType>>, "");
    static_assert(is_implemented<b_impl<MyType, MyType>>, "");
}

namespace priority_1 {
    // Make sure that a method specialized without `when` has priority over a
    // method specialized with `when` and a method specialized with `when`
    // and the list of unavailable methods.
    BOOST_HANA_METHOD(a_impl);

    template <typename T>
    struct a_impl<T, when<true>> { };

    template <typename T, typename _>
    struct a_impl<T, when<true>, _> { };

    struct MyType;
    template <>
    struct a_impl<MyType> {
        static constexpr bool a_MyType = true;
    };

    template <typename T>
    struct MyParametricType;
    template <typename T>
    struct a_impl<MyParametricType<T>> {
        static constexpr bool a_MyParametricType = true;
    };

    static_assert(dispatch<a_impl<MyType>>::a_MyType, "");
    static_assert(is_implemented<a_impl<MyType>>, "");

    static_assert(dispatch<a_impl<MyParametricType<void>>>::a_MyParametricType, "");
    static_assert(is_implemented<a_impl<MyParametricType<void>>>, "");
}

namespace priority_2 {
    // Make sure that a method specialized with a `when` has priority over a
    // method specialized with `when` plus the list of unavailable methods.
    BOOST_HANA_METHOD(a_impl);

    template <typename T, typename _>
    struct a_impl<T, when<true>, _> { };

    struct MyType;
    template <>
    struct a_impl<MyType, when<true>> {
        static constexpr bool a_MyType = true;
    };

    template <typename T>
    struct MyParametricType;
    template <typename T>
    struct a_impl<MyParametricType<T>, when<true>> {
        static constexpr bool a_MyParametricType = true;
    };

    static_assert(dispatch<a_impl<MyType>>::a_MyType, "");
    static_assert(is_implemented<a_impl<MyType>>, "");

    static_assert(dispatch<a_impl<MyParametricType<void>>>::a_MyParametricType, "");
    static_assert(is_implemented<a_impl<MyParametricType<void>>>, "");
}

namespace circular_MCD {
    // Make sure this case works as expected:
    // a(T) implemented by b(T)
    // b(T) implemented by a(T)
    //
    // a(mytype1) implemented
    // b(mytype2) implemented
    // ------
    // a(mytype1) and b(mytype1) should use the implemented a(mytype1)
    // a(mytype2) and b(mytype2) should use the implemented b(mytype2)
    BOOST_HANA_METHOD(a_impl);
    BOOST_HANA_METHOD(b_impl);

    template <typename T, typename _>
    struct a_impl<T, when<is_implemented<b_impl<T>, _>>, _> {
        static constexpr bool a_from_b = true;
    };

    template <typename T, typename _>
    struct b_impl<T, when<is_implemented<a_impl<T>, _>>, _> {
        static constexpr bool b_from_a = true;
    };


    struct MyType1;
    template <>
    struct a_impl<MyType1> {
        static constexpr bool a_MyType1 = true;
    };

    struct MyType2;
    template <>
    struct b_impl<MyType2> {
        static constexpr bool b_MyType2 = true;
    };

    static_assert(dispatch<a_impl<MyType1>>::a_MyType1, "");
    static_assert(dispatch<b_impl<MyType1>>::b_from_a, "");

    static_assert(dispatch<a_impl<MyType2>>::a_from_b, "");
    static_assert(dispatch<b_impl<MyType2>>::b_MyType2, "");
}

namespace Functor_Applicative_MCD {
    // Make sure this case works as expected:
    // transform(T) implemented by adjust(T)
    // adjust(T) implemented by transform(T)
    //
    // transform(T) also implemented by [lift(T) and ap(T)]
    //
    // transform(mytype1) implemented
    // adjust(mytype2) implemented
    // lift(mytype3) and ap(mytype3) implemented
    // ------
    // transform(mytype1) and adjust(mytype1) should use the implemented transform(mytype1)
    // lift(mytype1) and ap(mytype1) should not be implemented
    //
    // transform(mytype2) and adjust(mytype2) should use the implemented adjust(mytype2)
    // lift(mytype2) and ap(mytype2) should not be implemented
    //
    // transform(mytype3) and adjust(mytype3) should use the implemented lift(mytype3) and ap(mytype3)
    BOOST_HANA_METHOD(transform_impl);
    BOOST_HANA_METHOD(adjust_impl);
    BOOST_HANA_METHOD(lift_impl);
    BOOST_HANA_METHOD(ap_impl);

    template <typename T, typename _>
    struct transform_impl<T, when<is_implemented<adjust_impl<T>, _>>, _> {
        static constexpr bool transform_from_adjust = true;
    };

    template <typename T, typename _>
    struct adjust_impl<T, when<is_implemented<transform_impl<T>, _>>, _> {
        static constexpr bool adjust_from_transform = true;
    };

    template <typename T, typename _>
    struct transform_impl<T, when<
        is_implemented<lift_impl<T>, _> && is_implemented<ap_impl<T>, _>
    >, _> {
        static constexpr bool transform_from_lift_and_ap = true;
    };

    struct MyType1;
    template <>
    struct transform_impl<MyType1> {
        static constexpr bool transform_MyType1 = true;
    };

    struct MyType2;
    template <>
    struct adjust_impl<MyType2> {
        static constexpr bool adjust_MyType2 = true;
    };

    struct MyType3;
    template <>
    struct lift_impl<MyType3> {
        static constexpr bool lift_MyType3 = true;
    };
    template <>
    struct ap_impl<MyType3> {
        static constexpr bool ap_MyType3 = true;
    };

    static_assert(dispatch<transform_impl<MyType1>>::transform_MyType1, "");
    static_assert(dispatch<adjust_impl<MyType1>>::adjust_from_transform, "");
    static_assert(!is_implemented<lift_impl<MyType1>>, "");
    static_assert(!is_implemented<ap_impl<MyType1>>, "");

    static_assert(dispatch<transform_impl<MyType2>>::transform_from_adjust, "");
    static_assert(dispatch<adjust_impl<MyType2>>::adjust_MyType2, "");
    static_assert(!is_implemented<lift_impl<MyType2>>, "");
    static_assert(!is_implemented<ap_impl<MyType2>>, "");

    static_assert(dispatch<transform_impl<MyType3>>::transform_from_lift_and_ap, "");
    static_assert(dispatch<adjust_impl<MyType3>>::adjust_from_transform, "");
    static_assert(dispatch<lift_impl<MyType3>>::lift_MyType3, "");
    static_assert(dispatch<ap_impl<MyType3>>::ap_MyType3, "");
}

int main() { }
