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

// note: `is_O1` implies `is_implemented`, so `is_implemented && is_O1 === is_O1`.
struct not_implemented {
    static constexpr bool is_implemented = false;
    static constexpr bool is_O1 = false;
    template <typename ...X>
    static constexpr auto apply(X ...)
    { static_assert(sizeof...(X) && false, "not implemented"); }
};

template <template <typename ...> class f>
struct quote {
    template <typename ...x>
    using unquote = f<x...>;
};

template <typename This, typename T, typename AlreadyVisited>
struct method_info {
private:
    template <typename Method>
    using wrap = detail::if_t<detail::elem<typename Method::tag, AlreadyVisited>,
        not_implemented,
        typename Method::template impl<T, method_info<typename Method::tag, T, detail::append_t<AlreadyVisited, This>>>
    >;

public:
    template <typename M>
    constexpr auto operator()(M const&) const
    { return wrap<M>{}; }
};

namespace detail {
    template <template <typename ...> class Impl, typename M>
    struct method_impl : M {
        constexpr method_impl(M m) : M(m) { }
        template <typename ...x>
        using impl = Impl<x...>;
        using tag = quote<Impl>;
    };

    template <template <typename ...> class impl>
    struct make_method {
        template <typename M>
        constexpr auto operator()(M m) const
        { return method_impl<impl, M>{m}; }
    };
}

template <template <typename ...> class impl>
constexpr detail::make_method<impl> method{};

#define BOOST_HANA_METHOD_IMPL(name)                                                                            \
    template <typename, typename, typename> struct name ## _impl;                                               \
    template <typename T, typename Info = method_info<quote<name ## _impl>, T, detail::types<>>, typename = void>\
    struct name ## _impl : name ## _impl<T, Info, hana::when<true>> { };                                        \
                                                                                                                \
    template <typename T, typename Info, bool condition>                                                        \
    struct name ## _impl<T, Info, hana::when<condition>> : not_implemented { }                                  \
/**/


//////////////////////////////////////////////////////////////////////////////
// foldable/foldable.hpp

// foldl
BOOST_HANA_METHOD_IMPL(foldl);
auto foldl = method<foldl_impl>([](auto xs, auto s, auto f) {
    return foldl_impl<hana::datatype_t<decltype(xs)>>::apply(xs, s, f);
});

// foldr
BOOST_HANA_METHOD_IMPL(foldr);
auto foldr = method<foldr_impl>([](auto xs, auto s, auto f) {
    return foldr_impl<hana::datatype_t<decltype(xs)>>::apply(xs, s, f);
});

// unpack
BOOST_HANA_METHOD_IMPL(unpack);
auto unpack = method<unpack_impl>([](auto xs, auto f) {
    return unpack_impl<hana::datatype_t<decltype(xs)>>::apply(xs, f);
});

// length
BOOST_HANA_METHOD_IMPL(length);
auto length = method<length_impl>([](auto xs) {
    return length_impl<hana::datatype_t<decltype(xs)>>::apply(xs);
});

// count
BOOST_HANA_METHOD_IMPL(count);
auto count = method<count_impl>([](auto xs, auto pred) {
    return count_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred);
});


//////////////////////////////////////////////////////////////////////////////
// functor/functor.hpp

// fmap
BOOST_HANA_METHOD_IMPL(fmap);
auto fmap = method<fmap_impl>([](auto xs, auto f) {
    return fmap_impl<hana::datatype_t<decltype(xs)>>::apply(xs, f);
});

// adjust
BOOST_HANA_METHOD_IMPL(adjust);
auto adjust = method<adjust_impl>([](auto xs, auto pred, auto f) {
    return adjust_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred, f);
});

// replace
BOOST_HANA_METHOD_IMPL(replace);
auto replace = method<replace_impl>([](auto xs, auto pred, auto value) {
    return replace_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred, value);
});

// fill
BOOST_HANA_METHOD_IMPL(fill);
auto fill = method<fill_impl>([](auto xs, auto value) {
    return fill_impl<hana::datatype_t<decltype(xs)>>::apply(xs, value);
});

//////////////////////////////////////////////////////////////////////////////
// applicative/applicative.hpp

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
constexpr auto lift = method<lift_impl>(detail::lift<T>{});

// ap
BOOST_HANA_METHOD_IMPL(ap);
auto ap = method<ap_impl>([](auto f, auto x) {
    using F = hana::datatype_t<decltype(f)>;
    using X = hana::datatype_t<decltype(x)>;
    static_assert(std::is_same<F, X>::value,
    "ap must be called with two arguments in the same Applicative");
    return ap_impl<F>::apply(f, x);
});

//////////////////////////////////////////////////////////////////////////////
// monad/monad.hpp

// flatten
BOOST_HANA_METHOD_IMPL(flatten);
auto flatten = method<flatten_impl>([](auto m) {
    return flatten_impl<hana::datatype_t<decltype(m)>>::apply(m);
});


//////////////////////////////////////////////////////////////////////////////
// foldable/provided.hpp
template <typename T, typename Info>
struct foldr_impl<T, Info, hana::when<
    Info{}(unpack).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false; // variadic::foldr is O(n)
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return unpack(xs, [=](auto ...xs) {
            return hana::detail::variadic::foldr(f, s, xs...);
        });
    }
};

template <typename T, typename Info>
struct foldl_impl<T, Info, hana::when<
    Info{}(unpack).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false; // variadic::foldl is O(n)
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return unpack(xs, [=](auto ...xs) {
            return hana::detail::variadic::foldl(f, s, xs...);
        });
    }
};

template <typename T, typename Info>
struct unpack_impl<T, Info, hana::when<
    Info{}(foldl).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(foldl).is_O1;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return foldl(xs, f, hana::partial)();
    }
};

template <typename T, typename Info>
struct length_impl<T, Info, hana::when<
    Info{}(foldl).is_implemented &&
    !Info{}(unpack).is_O1
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(foldl).is_O1;
    template <typename Xs>
    static constexpr auto apply(Xs xs) {
        return foldl(xs, hana::int_<0>, [](auto n, auto _) {
            return hana::plus(n, hana::int_<1>);
        });
    }
};

template <typename T, typename Info>
struct length_impl<T, Info, hana::when<
    Info{}(unpack).is_O1
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(unpack).is_O1;
    template <typename Xs>
    static constexpr auto apply(Xs xs) {
        return unpack(xs, [](auto ...xs) {
            return hana::size_t<sizeof...(xs)>;
        });
    }
};

template <typename T, typename Info>
struct count_impl<T, Info, hana::when<
    Info{}(foldl).is_implemented &&
    !Info{}(unpack).is_O1
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(foldl).is_O1;
    template <typename Xs, typename Pred>
    static constexpr auto apply(Xs xs, Pred pred) {
        return foldl(xs, hana::size_t<0>, [=](auto counter, auto x) {
            return hana::if_(pred(x),
                hana::plus(counter, hana::size_t<1>),
                counter
            );
        });
    }
};

template <typename T, typename Info>
struct count_impl<T, Info, hana::when<
    Info{}(unpack).is_O1
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false; // this implementation is still O(n)
    template <typename Xs, typename Pred>
    static constexpr auto apply(Xs xs, Pred pred) {
        // The optimization is not applied right now, but we could do
        // something like:
#if 0
        // This is valid assuming a correct definition of Boolean which
        // requires instances to be convertible to `bool`.
        return unpack(xs, [](auto ...xs) {
            constexpr bool cs[] = {false, to<bool>(pred(xs))...};
            return size_t<count(cs)>;
        });
#endif
        return foldl(xs, hana::size_t<0>, [=](auto counter, auto x) {
            return hana::if_(pred(x),
                hana::plus(counter, hana::size_t<1>),
                counter
            );
        });
    }
};

//////////////////////////////////////////////////////////////////////////////
// functor/provided.hpp

template <typename T, typename Info>
struct fmap_impl<T, Info, hana::when<
    Info{}(adjust).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(adjust).is_O1;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return adjust(xs, hana::always(hana::true_), f);
    }
};

template <typename T, typename Info>
struct adjust_impl<T, Info, hana::when<
    Info{}(fmap).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(fmap).is_O1;
    template <typename Xs, typename Pred, typename F>
    static constexpr auto apply(Xs xs, Pred pred, F f) {
        return fmap(xs, [=](auto x) {
            return hana::if_(pred(x), f(x), x);
        });
    }
};

template <typename T, typename Info>
struct fill_impl<T, Info, hana::when<
    Info{}(fmap).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(fmap).is_O1;
    template <typename Xs, typename V>
    static constexpr auto apply(Xs xs, V value) {
        return fmap(xs, hana::always(value));
    }
};

template <typename T, typename Info>
struct replace_impl<T, Info, hana::when<
    Info{}(adjust).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(adjust).is_O1;
    template <typename Xs, typename Pred, typename V>
    static constexpr auto apply(Xs xs, Pred pred, V value) {
        return adjust(xs, pred, hana::always(value));
    }
};


//////////////////////////////////////////////////////////////////////////////
// applicative/provided.hpp

template <typename T, typename Info>
struct fmap_impl<T, Info, hana::when<
    Info{}(lift<T>).is_implemented &&
    Info{}(ap).is_implemented
>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = Info{}(lift<T>).is_O1 && Info{}(ap).is_O1;
    template <typename X, typename F>
    static constexpr auto apply(X x, F f) {
        return ap(lift<T>(f), x);
    }
};


//////////////////////////////////////////////////////////////////////////////
// tuple/foldable.hpp

template <typename Info>
struct unpack_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = true;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return hana::unpack(xs, f);
    }
};

template <typename Info>
struct foldr_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return hana::foldr(xs, s, f);
    }
};

template <typename Info>
struct foldl_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return hana::foldl(xs, s, f);
    }
};


//////////////////////////////////////////////////////////////////////////////
// tuple/functor.hpp

// redundant given the Applicative instance
#if 0
template <typename Info>
struct fmap_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return hana::fmap(f, xs);
    }
};
#endif

//////////////////////////////////////////////////////////////////////////////
// tuple/applicative.hpp

template <typename Info>
struct lift_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = true;
    template <typename X>
    static constexpr auto apply(X x) {
        return hana::tuple(x);
    }
};

template <typename Info>
struct ap_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Fs, typename Xs>
    static constexpr auto apply(Fs fs, Xs xs) {
        return hana::ap(fs, xs);
    }
};

//////////////////////////////////////////////////////////////////////////////
// tuple/monad.hpp

template <typename Info>
struct flatten_impl<hana::Tuple, Info> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Xs>
    static constexpr auto apply(Xs xs) {
        return hana::flatten(xs);
    }
};

//////////////////////////////////////////////////////////////////////////////


int main() {
    auto xs = hana::tuple(1, '2', 3.3, 4.4f, nullptr);

    // Foldable
    unpack(xs, [](auto ...xs) { });
    foldr(xs, 1, [](auto x, auto s) { return s; });
    foldl(xs, 1, [](auto s, auto x) { return s; });
    length(xs);
    count(xs, [](auto) { return hana::true_; });


    // Functor
    fmap(xs, [](auto x) { return 1; });
    replace(xs, [](auto x) { return hana::true_; }, 1);
    adjust(xs, [](auto x) { return hana::true_; }, [](auto x) { return x; });
    fill(xs, 1);

    // Applicative
    lift<hana::Tuple>(1);
    ap(hana::tuple([](auto x) { return x; }), xs);

    // Monad
    flatten(hana::tuple(xs, xs));
}
