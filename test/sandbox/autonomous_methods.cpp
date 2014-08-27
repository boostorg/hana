/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>

#include <type_traits>
namespace hana = boost::hana;


// note: `is_O1` implies `is_implemented`, so `is_implemented && is_O1 === is_O1`.
struct not_implemented {
    static constexpr bool is_implemented = false;
    static constexpr bool is_O1 = false;
    template <typename ...X>
    static constexpr auto apply(X ...)
    { static_assert(sizeof...(X) && false, "not implemented"); }
};


//////////////////////////////////////////////////////////////////////////////
// foldable/foldable.hpp

// foldl
template <typename T, typename = void>
struct foldl_impl : foldl_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct foldl_impl<T, hana::when<condition>> : not_implemented { };

auto foldl = [](auto xs, auto s, auto f) {
    return foldl_impl<hana::datatype_t<decltype(xs)>>::apply(xs, s, f);
};

// foldr
template <typename T, typename = void>
struct foldr_impl : foldr_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct foldr_impl<T, hana::when<condition>> : not_implemented { };

auto foldr = [](auto xs, auto s, auto f) {
    return foldr_impl<hana::datatype_t<decltype(xs)>>::apply(xs, s, f);
};

// unpack
template <typename T, typename = void>
struct unpack_impl : unpack_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct unpack_impl<T, hana::when<condition>> : not_implemented { };

auto unpack = [](auto xs, auto f) {
    return unpack_impl<hana::datatype_t<decltype(xs)>>::apply(xs, f);
};

// length
template <typename T, typename = void>
struct length_impl : length_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct length_impl<T, hana::when<condition>> : not_implemented { };

auto length = [](auto xs) {
    return length_impl<hana::datatype_t<decltype(xs)>>::apply(xs);
};

// count
template <typename T, typename = void>
struct count_impl : count_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct count_impl<T, hana::when<condition>> : not_implemented { };

auto count = [](auto xs, auto pred) {
    return count_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred);
};


//////////////////////////////////////////////////////////////////////////////
// functor/functor.hpp

// fmap
template <typename T, typename = void>
struct fmap_impl : fmap_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct fmap_impl<T, hana::when<condition>> : not_implemented { };

auto fmap = [](auto xs, auto f) {
    return fmap_impl<hana::datatype_t<decltype(xs)>>::apply(xs, f);
};

// adjust
template <typename T, typename = void>
struct adjust_impl : adjust_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct adjust_impl<T, hana::when<condition>> : not_implemented { };

auto adjust = [](auto xs, auto pred, auto f) {
    return adjust_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred, f);
};

// replace
template <typename T, typename = void>
struct replace_impl : replace_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct replace_impl<T, hana::when<condition>> : not_implemented { };

auto replace = [](auto xs, auto pred, auto value) {
    return replace_impl<hana::datatype_t<decltype(xs)>>::apply(xs, pred, value);
};

// fill
template <typename T, typename = void>
struct fill_impl : fill_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct fill_impl<T, hana::when<condition>> : not_implemented { };

auto fill = [](auto xs, auto value) {
    return fill_impl<hana::datatype_t<decltype(xs)>>::apply(xs, value);
};

//////////////////////////////////////////////////////////////////////////////
// applicative/applicative.hpp

// lift
template <typename T, typename = void>
struct lift_impl : lift_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct lift_impl<T, hana::when<condition>> : not_implemented { };

template <typename T>
auto lift = [](auto x) {
    return lift_impl<T>::apply(x);
};

// ap
template <typename T, typename = void>
struct ap_impl : ap_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct ap_impl<T, hana::when<condition>> : not_implemented { };

auto ap = [](auto f, auto x) {
    using F = hana::datatype_t<decltype(f)>;
    using X = hana::datatype_t<decltype(x)>;
    static_assert(std::is_same<F, X>::value,
    "ap must be called with two arguments in the same Applicative");
    return ap_impl<F>::apply(f, x);
};

//////////////////////////////////////////////////////////////////////////////
// monad/monad.hpp

// flatten
template <typename T, typename = void>
struct flatten_impl : flatten_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct flatten_impl<T, hana::when<condition>> : not_implemented { };

auto flatten = [](auto m) {
    return flatten_impl<hana::datatype_t<decltype(m)>>::apply(m);
};


//////////////////////////////////////////////////////////////////////////////
// foldable/provided.hpp
template <typename T>
struct foldr_impl<T, hana::when<unpack_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false; // variadic::foldr is O(n)
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return unpack(xs, [=](auto ...xs) {
            return hana::detail::variadic::foldr(f, s, xs...);
        });
    }
};

template <typename T>
struct foldl_impl<T, hana::when<unpack_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false; // variadic::foldl is O(n)
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return unpack(xs, [=](auto ...xs) {
            return hana::detail::variadic::foldl(f, s, xs...);
        });
    }
};

template <typename T>
struct unpack_impl<T, hana::when<foldl_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = foldl_impl<T>::is_O1;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return foldl(xs, f, hana::partial)();
    }
};

template <typename T>
struct length_impl<T, hana::when<foldl_impl<T>::is_implemented && !unpack_impl<T>::is_O1>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = foldl_impl<T>::is_O1;
    template <typename Xs>
    static constexpr auto apply(Xs xs) {
        return foldl(xs, hana::int_<0>, [](auto n, auto _) {
            return hana::plus(n, hana::int_<1>);
        });
    }
};

template <typename T>
struct length_impl<T, hana::when<unpack_impl<T>::is_O1>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = unpack_impl<T>::is_O1;
    template <typename Xs>
    static constexpr auto apply(Xs xs) {
        return unpack(xs, [](auto ...xs) {
            return hana::size_t<sizeof...(xs)>;
        });
    }
};

template <typename T>
struct count_impl<T, hana::when<foldl_impl<T>::is_implemented && !unpack_impl<T>::is_O1>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = foldl_impl<T>::is_O1;
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

template <typename T>
struct count_impl<T, hana::when<unpack_impl<T>::is_O1>> {
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

template <typename T>
struct fmap_impl<T, hana::when<adjust_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = adjust_impl<T>::is_O1;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return adjust(xs, hana::always(hana::true_), f);
    }
};

template <typename T>
struct adjust_impl<T, hana::when<fmap_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = fmap_impl<T>::is_O1;
    template <typename Xs, typename Pred, typename F>
    static constexpr auto apply(Xs xs, Pred pred, F f) {
        return fmap(xs, [=](auto x) {
            return hana::if_(pred(x), f(x), x);
        });
    }
};

template <typename T>
struct fill_impl<T, hana::when<fmap_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = fmap_impl<T>::is_O1;
    template <typename Xs, typename V>
    static constexpr auto apply(Xs xs, V value) {
        return fmap(xs, hana::always(value));
    }
};

template <typename T>
struct replace_impl<T, hana::when<adjust_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = adjust_impl<T>::is_O1;
    template <typename Xs, typename Pred, typename V>
    static constexpr auto apply(Xs xs, Pred pred, V value) {
        return adjust(xs, pred, hana::always(value));
    }
};


//////////////////////////////////////////////////////////////////////////////
// applicative/provided.hpp

template <typename T>
struct fmap_impl<T, hana::when<lift_impl<T>::is_implemented && ap_impl<T>::is_implemented>> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = lift_impl<T>::is_O1 && ap_impl<T>::is_O1;
    template <typename X, typename F>
    static constexpr auto apply(X x, F f) {
        return ap(lift<T>(f), x);
    }
};


//////////////////////////////////////////////////////////////////////////////
// tuple/foldable.hpp

template <>
struct unpack_impl<hana::Tuple> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = true;
    template <typename Xs, typename F>
    static constexpr auto apply(Xs xs, F f) {
        return hana::unpack(xs, f);
    }
};

template <>
struct foldr_impl<hana::Tuple> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Xs, typename S, typename F>
    static constexpr auto apply(Xs xs, S s, F f) {
        return hana::foldr(xs, s, f);
    }
};

template <>
struct foldl_impl<hana::Tuple> {
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
template <>
struct fmap_impl<hana::Tuple> {
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

template <>
struct lift_impl<hana::Tuple> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = true;
    template <typename X>
    static constexpr auto apply(X x) {
        return hana::tuple(x);
    }
};

template <>
struct ap_impl<hana::Tuple> {
    static constexpr bool is_implemented = true;
    static constexpr bool is_O1 = false;
    template <typename Fs, typename Xs>
    static constexpr auto apply(Fs fs, Xs xs) {
        return hana::ap(fs, xs);
    }
};

//////////////////////////////////////////////////////////////////////////////
// tuple/monad.hpp

template <>
struct flatten_impl<hana::Tuple> {
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
    // There's a circular dependency between those and fmap.
    // replace(xs, [](auto x) { return hana::true_; }, 1);
    // adjust(xs, [](auto x) { return hana::true_; }, [](auto x) { return x; });
    fill(xs, 1);

    // Applicative
    lift<hana::Tuple>(1);
    ap(hana::tuple([](auto x) { return x; }), xs);

    // Monad
    flatten(hana::tuple(xs, xs));
}
