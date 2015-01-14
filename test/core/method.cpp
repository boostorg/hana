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

namespace fictive_Foldable {
    // foldl
    BOOST_HANA_METHOD(foldl_impl);
    auto foldl = [](auto xs, auto s, auto f) {
        return foldl_impl<datatype_t<decltype(xs)>>::apply(xs, s, f);
    };

    // unpack
    BOOST_HANA_METHOD(unpack_impl);
    auto unpack = [](auto xs, auto f) {
        return unpack_impl<datatype_t<decltype(xs)>>::apply(xs, f);
    };

    template <typename T, typename _>
    struct foldl_impl<T, _, when<
        is_implemented<unpack_impl<T>, _>
    >> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) { }
    };

    template <typename T, typename _>
    struct unpack_impl<T, _, when<
        is_implemented<foldl_impl<T>, _>
    >> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) { }
    };

    struct MyType1 { };
    struct MyType2 { };

    template <typename _>
    struct unpack_impl<MyType1, _> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) { }
    };

    template <typename _>
    struct foldl_impl<MyType2, _> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) { }
    };

    void test() {
        unpack(MyType1{}, [](auto ...xs) { });
        foldl(MyType1{}, 1, [](auto s, auto x) { return s; });

        unpack(MyType2{}, [](auto ...xs) { });
        foldl(MyType2{}, 1, [](auto s, auto x) { return s; });
    }
}

namespace fictive_Functor_Applicative {
    ////////////////////
    // Functor

    // fmap
    BOOST_HANA_METHOD(fmap_impl);
    auto fmap = [](auto xs, auto f) {
        return fmap_impl<datatype_t<decltype(xs)>>::apply(xs, f);
    };

    // adjust
    BOOST_HANA_METHOD(adjust_impl);
    auto adjust = [](auto xs, auto pred, auto f) {
        return adjust_impl<datatype_t<decltype(xs)>>::apply(xs, pred, f);
    };


    template <typename T, typename _>
    struct fmap_impl<T, _, when<
        is_implemented<adjust_impl<T>, _>
    >> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) { }
    };

    template <typename T, typename _>
    struct adjust_impl<T, _, when<
        is_implemented<fmap_impl<T>, _>
    >> {
        template <typename Xs, typename Pred, typename F>
        static constexpr auto apply(Xs xs, Pred pred, F f) { }
    };

    ////////////////////
    // Applicative

    // lift
    BOOST_HANA_METHOD(lift_impl);
    namespace detail {
        template <typename T>
        struct lift {
            template <typename X>
            constexpr auto operator()(X x) const
            { return lift_impl<T>::apply(x); }
        };
    }
    template <typename T>
    constexpr auto lift = detail::lift<T>{};

    // ap
    BOOST_HANA_METHOD(ap_impl);
    auto ap = [](auto f, auto x) {
        return ap_impl<datatype_t<decltype(f)>>::apply(f, x);
    };


    template <typename T, typename _>
    struct fmap_impl<T, _, when<
        is_implemented<lift_impl<T>, _> &&
        is_implemented<ap_impl<T>, _>
    >> {
        template <typename X, typename F>
        static constexpr auto apply(X x, F f) { }
    };



    struct MyType1 { };
    template <typename _>
    struct fmap_impl<MyType1, _> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) { }
    };

    struct MyType3 { };
    template <typename _>
    struct adjust_impl<MyType3, _> {
        template <typename Xs, typename Pred, typename F>
        static constexpr auto apply(Xs xs, Pred pred, F f) { }
    };


    struct MyType2 { };
    template <typename _>
    struct lift_impl<MyType2, _> {
        template <typename X>
        static constexpr auto apply(X x) { }
    };
    template <typename _>
    struct ap_impl<MyType2, _> {
        template <typename Fs, typename Xs>
        static constexpr auto apply(Fs fs, Xs xs) { }
    };


    void test() {
        fmap(MyType1{}, [](auto x) { return 1; });
        adjust(MyType1{}, [](auto x) { return true; }, [](auto x) { return 1; });

        fmap(MyType3{}, [](auto x) { return 1; });
        adjust(MyType3{}, [](auto x) { return true; }, [](auto x) { return 1; });

        fmap(MyType2{}, [](auto x) { return 1; });
        adjust(MyType2{}, [](auto x) { return true; }, [](auto x) { return 1; });
        lift<MyType2>(1);
        ap(MyType2{}, MyType2{});
    }
}

int main() { }
