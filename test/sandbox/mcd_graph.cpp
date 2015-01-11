/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>

#include <type_traits>
namespace hana = boost::hana;


// This is an attempt to
// 1. split methods out of type classes
// 2. break cyclic dependencies that arise when several minimal complete
//    definitions are possible.
//
// For example,
//
//      method1 => method2
//      method2 => method1
//
// where `=>` means "provides". This will cause the partial specializations
// to recurse indefinitely. What we do here is keep track of the methods that
// were seen so far and disable (by hand) the specializations that would
// cause a cycle.
//
// This works, but has drawbacks:
//  1. likely inefficient
//  2. a lot of boilerplate for each method and method specialization
//
//
// By the way, the problem can be formally stated as finding a path ending
// at a vertex without out-edges in a graph. The vertices are methods and
// an edge from `a` to `b` means that method `a` provides method `b`. The
// difficulty is not to get caught in cycles, hence the need to remember
// all the nodes seen so far.

namespace detail {
    template <typename T>
    struct nodecay { };

    void void_(...);

    template <typename ...T>
    struct inherit : T... { };

    // Note that all Ts must be unique.
    template <typename ...T>
    struct types { };


    // all
    template <bool ...conditions>
    struct all {
        static constexpr bool value = hana::detail::std::is_same<
            all, all<(conditions, true)...>
        >::value;
    };


    // any
    template <bool ...conditions>
    struct any {
        static constexpr bool value = !hana::detail::std::is_same<
            any, any<(conditions, false)...>
        >::value;
    };


    // none
    template <bool ...conditions>
    struct none {
        static constexpr bool value = !any<conditions...>::value;
    };


    // concat
    template <typename Ts, typename Us>
    struct concat;

    template <typename ...T, typename ...U>
    struct concat<types<T...>, types<U...>>
    { using type = types<T..., U...>; };

    template <typename Ts, typename Us>
    using concat_t = typename concat<Ts, Us>::type;


    // append
    template <typename Ts, typename ...U>
    struct append;

    template <typename ...T, typename ...U>
    struct append<types<T...>, U...>
    { using type = types<T..., U...>; };

    template <typename Ts, typename ...U>
    using append_t = typename append<Ts, U...>::type;


    // is_subset
    template <typename Ts, typename Us, typename = void>
    struct is_subset { static constexpr bool value = false; };

    template <typename ...T, typename ...U>
    struct is_subset<types<T...>, types<U...>, decltype(void_(
        static_cast<nodecay<T>>(inherit<nodecay<U>...>{})...
    ))> { static constexpr bool value = true; };


    // elem
    template <typename T, typename Us, typename = void>
    struct elem { static constexpr bool value = false; };

    template <typename T, typename ...U>
    struct elem<T, types<U...>, decltype((void)(
        static_cast<nodecay<T>>(inherit<nodecay<U>...>{})
    ))> { static constexpr bool value = true; };


    // is_disjoint
    template <typename Ts, typename Us>
    struct is_disjoint;

    template <typename ...T, typename Us>
    struct is_disjoint<types<T...>, Us> {
        static constexpr bool value = none<elem<T, Us>::value...>::value;
    };


    template <bool b>
    struct helper {
        template <typename X, typename Y>
        using and_ = Y;

        template <typename X, typename Y>
        using or_ = X;

        template <typename T, typename E>
        using if_ = T;
    };

    template <>
    struct helper<false> {
        template <typename X, typename Y>
        using and_ = X;

        template <typename X, typename Y>
        using or_ = Y;

        template <typename T, typename E>
        using if_ = E;
    };


    // lazy_and
    template <typename X, typename Y>
    struct lazy_and {
        static constexpr bool value = helper<X::value>::
                                      template and_<X, Y>::value;
    };

    // lazy_or
    template <typename X, typename Y>
    struct lazy_or {
        static constexpr bool value = helper<X::value>::
                                      template or_<X, Y>::value;
    };

    // lazy_not
    template <typename X>
    struct lazy_not { static constexpr bool value = !X::value; };


    // if_
    template <typename C, typename T, typename E>
    struct if_ {
        using type = typename helper<C::value>::template if_<T, E>;
    };

    template <typename C, typename T, typename E>
    using if_t = typename if_<C, T, E>::type;
}


struct not_implemented {
    template <typename ...X>
    static constexpr auto apply(X ...)
    { static_assert(sizeof...(X) && false, "not implemented"); }
};

template <template <typename ...> class f, typename ...t>
struct quote;

constexpr bool is_implemented_impl(not_implemented) { return false; }
constexpr bool is_implemented_impl(...) { return true; }

template <typename Method, typename Dependencies>
struct is_implemented_impl2;

// Unary methods
template <template <typename ...> class Method, typename T, typename ..._, typename ...Dependencies>
struct is_implemented_impl2<Method<T, detail::types<quote<Method, T>>, _...>, detail::types<Dependencies...>> {
    using type = detail::if_t<detail::elem<quote<Method, T>, detail::types<Dependencies...>>,
        not_implemented,
        Method<T, detail::types<quote<Method, T>, Dependencies...>, _...>
    >;
    static constexpr bool value = is_implemented_impl(type{});
};

// Binary methods
template <template <typename ...> class Method, typename T, typename U, typename ..._, typename ...Dependencies>
struct is_implemented_impl2<Method<T, U, detail::types<quote<Method, T, U>>, _...>, detail::types<Dependencies...>> {
    using type = detail::if_t<detail::elem<quote<Method, T, U>, detail::types<Dependencies...>>,
        not_implemented,
        Method<T, U, detail::types<quote<Method, T, U>, Dependencies...>, _...>
    >;
    static constexpr bool value = is_implemented_impl(type{});
};

template <typename Method, typename Dependencies = detail::types<>>
constexpr bool is_implemented = is_implemented_impl2<Method, Dependencies>::value;


#define BOOST_HANA_METHOD_IMPL(name)                                                        \
    template <typename, typename, typename> struct name ## _impl;                           \
    template <typename T, typename _ = detail::types<quote<name ## _impl, T>>, typename = void>  \
    struct name ## _impl : name ## _impl<T, _, hana::when<true>> { };                       \
                                                                                            \
    template <typename T, typename _, bool condition>                                       \
    struct name ## _impl<T, _, hana::when<condition>> : not_implemented { }                 \
/**/

#define BOOST_HANA_BINARY_METHOD_IMPL(name)                                     \
    template <typename, typename, typename, typename> struct name ## _impl;     \
    template <typename T, typename U, typename _ = detail::types<quote<name ## _impl, T, U>>, typename = void>  \
    struct name ## _impl : name ## _impl<T, U, _, hana::when<true>> { };        \
                                                                                \
    template <typename T, typename U, typename _, bool condition>               \
    struct name ## _impl<T, U, _, hana::when<condition>> : not_implemented { }  \
/**/



//////////////////////////////////////////////////////////////////////////////
// Foldable
//////////////////////////////////////////////////////////////////////////////

// foldl
BOOST_HANA_METHOD_IMPL(foldl);
auto foldl = [](auto xs, auto s, auto f) {
    return foldl_impl<hana::datatype_t<decltype(xs)>>::apply(xs, s, f);
};

// unpack
BOOST_HANA_METHOD_IMPL(unpack);
auto unpack = [](auto xs, auto f) {
    return unpack_impl<hana::datatype_t<decltype(xs)>>::apply(xs, f);
};

template <typename T, typename _>
struct foldl_impl<T, _, hana::when<
    is_implemented<unpack_impl<T>, _>
>> {
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return unpack(xs, [=](auto ...xs) {
            return hana::detail::variadic::foldl(f, s, xs...);
        });
    }
};

template <typename T, typename _>
struct unpack_impl<T, _, hana::when<
    is_implemented<foldl_impl<T>, _>
>> {
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return foldl(xs, f, hana::partial)();
    }
};


//////////////////////////////////////////////////////////////////////////////
// Functor
//////////////////////////////////////////////////////////////////////////////

// fmap
BOOST_HANA_METHOD_IMPL(fmap);
auto fmap = [](auto xs, auto f) {
    return fmap_impl<hana::datatype_t<decltype(xs)>>::apply(xs, f);
};

// adjust
BOOST_HANA_METHOD_IMPL(adjust);
auto adjust = [](auto xs, auto pred, auto f) {
    return adjust_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred, f);
};


template <typename T, typename _>
struct fmap_impl<T, _, hana::when<
    is_implemented<adjust_impl<T>, _>
>> {
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return adjust(xs, hana::always(hana::true_), f);
    }
};

template <typename T, typename _>
struct adjust_impl<T, _, hana::when<
    is_implemented<fmap_impl<T>, _>
>> {
    template <typename Xs, typename Pred, typename F>
    static constexpr auto apply(Xs xs, Pred pred, F f) {
        return fmap(xs, [=](auto x) {
            return hana::if_(pred(x), f(x), x);
        });
    }
};


//////////////////////////////////////////////////////////////////////////////
// Applicative
//////////////////////////////////////////////////////////////////////////////

// lift
BOOST_HANA_METHOD_IMPL(lift);
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
BOOST_HANA_METHOD_IMPL(ap);
auto ap = [](auto f, auto x) {
    using F = hana::datatype_t<decltype(f)>;
    using X = hana::datatype_t<decltype(x)>;
    static_assert(std::is_same<F, X>::value,
    "ap must be called with two arguments in the same Applicative");
    return ap_impl<F>::apply(f, x);
};


template <typename T, typename _>
struct fmap_impl<T, _, hana::when<
    is_implemented<lift_impl<T>, _> &&
    is_implemented<ap_impl<T>, _>
>> {
    template <typename X, typename F>
    static constexpr auto apply(X x, F f) {
        return ap(lift<T>(f), x);
    }
};


//////////////////////////////////////////////////////////////////////////////
// Monad
//////////////////////////////////////////////////////////////////////////////

// flatten
BOOST_HANA_METHOD_IMPL(flatten);
auto flatten = [](auto m) {
    return flatten_impl<hana::datatype_t<decltype(m)>>::apply(m);
};



//////////////////////////////////////////////////////////////////////////////
// Tuple
//////////////////////////////////////////////////////////////////////////////

// Foldable
#if 0
template <typename Info>
struct unpack_impl<hana::Tuple, Info> {
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return hana::unpack(xs, f);
    }
};
#else
template <typename Info>
struct foldl_impl<hana::Tuple, Info> {
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return hana::foldl(xs, s, f);
    }
};
#endif


// Functor (redundant given the Applicative instance)
#if 0
template <typename Info>
struct fmap_impl<hana::Tuple, Info> {
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return hana::fmap(xs, f);
    }
};
#endif


// Applicative
template <typename Info>
struct lift_impl<hana::Tuple, Info> {
    template <typename X>
    static constexpr auto apply(X x) {
        return hana::tuple(x);
    }
};

template <typename Info>
struct ap_impl<hana::Tuple, Info> {
    template <typename Fs, typename Xs>
    static constexpr auto apply(Fs fs, Xs xs) {
        return hana::ap(fs, xs);
    }
};

// Monad
template <typename Info>
struct flatten_impl<hana::Tuple, Info> {
    template <typename Xs>
    static constexpr auto apply(Xs xs) {
        return hana::flatten(xs);
    }
};


//////////////////////////////////////////////////////////////////////////////


namespace test_case_1 {
    // Le cas de base qu'on essaie de gérer est:
    //  a(T) implémenté par b(T)
    //  b(T) implémenté par a(T) ou par c(T)
    //  c(mytype) implémenté
    //  ------
    //  a(mytype) doit utiliser c(mytype) au lieu de récurser

    BOOST_HANA_METHOD_IMPL(a);
    BOOST_HANA_METHOD_IMPL(b);
    BOOST_HANA_METHOD_IMPL(c);


    template <typename T, typename _>
    struct a_impl<T, _, hana::when<
        is_implemented<b_impl<T>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    template <typename T, typename _>
    struct b_impl<T, _, hana::when<
        is_implemented<a_impl<T>, _>
    >> { };

    template <typename T, typename _>
    struct b_impl<T, _, hana::when<
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

namespace test_case_2 {
    // Version dégénérée du cas de base:
    //  a(T) implémenté par a(T) ou b(T)
    //  b(mytype) implémenté
    //  ------
    //  a(mytype) doit utiliser b(mytype) au lieu de récurser

    BOOST_HANA_METHOD_IMPL(a);
    BOOST_HANA_METHOD_IMPL(b);


    template <typename T, typename _>
    struct a_impl<T, _, hana::when<
        is_implemented<a_impl<T>, _>
    >> { };

    template <typename T, typename _>
    struct a_impl<T, _, hana::when<
        is_implemented<b_impl<T>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    struct MyType;
    template <typename _>
    struct b_impl<MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType>::dispatches_correctly, "");
    static_assert(b_impl<MyType>::dispatches_correctly, "");
}

namespace test_case_2_bin {
    // Version dégénérée du cas de base avec des méthodes binaires:
    //  a(T, U) implémenté par a(T, U) ou b(T, U)
    //  b(mytype, mytype) implémenté
    //  ------
    //  a(mytype, mytype) doit utiliser b(mytype, mytype) au lieu de récurser

    BOOST_HANA_BINARY_METHOD_IMPL(a);
    BOOST_HANA_BINARY_METHOD_IMPL(b);


    template <typename T, typename U, typename _>
    struct a_impl<T, U, _, hana::when<
        is_implemented<a_impl<T, U>, _>
    >> { };

    template <typename T, typename U, typename _>
    struct a_impl<T, U, _, hana::when<
        is_implemented<b_impl<T, U>, _>
    >> { static constexpr bool dispatches_correctly = true; };


    struct MyType;
    template <typename _>
    struct b_impl<MyType, MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType, MyType>::dispatches_correctly, "");
    static_assert(b_impl<MyType, MyType>::dispatches_correctly, "");
}

namespace test_case_3 {
    // S'assure que la priorité de la spécialisation sans `when` est
    // respectée même pour les types paramétriques. Ça pourrait chier
    // parce qu'on a rajouté le template parameter `_`.
    BOOST_HANA_METHOD_IMPL(a);

    template <typename T, typename _>
    struct a_impl<T, _, hana::when<true>> { };

    template <typename T> struct MyType;
    template <typename T, typename _>
    struct a_impl<MyType<T>, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType<void>>::dispatches_correctly, "");
}

namespace test_case_4 {
    // Expérimente avec des méthodes binaires.
    BOOST_HANA_BINARY_METHOD_IMPL(a);

    struct MyType;
    template <typename _>
    struct a_impl<MyType, MyType, _>
    { static constexpr bool dispatches_correctly = true; };

    static_assert(a_impl<MyType, MyType>::dispatches_correctly, "");
}




int main() {
    auto xs = hana::tuple(1, '2', 3.3, 4.4f, nullptr);

    // Foldable
    unpack(xs, [](auto ...xs) { });
    foldl(xs, 1, [](auto s, auto x) { return s; });


    // Functor
    fmap(xs, [](auto x) { return 1; });
    adjust(xs, [](auto x) { return hana::true_; }, [](auto x) { return x; });

    // Applicative
    lift<hana::Tuple>(1);
    ap(hana::tuple([](auto x) { return x; }), xs);

    // Monad
    flatten(hana::tuple(xs, xs));
}
