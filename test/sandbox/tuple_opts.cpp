/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/sandbox/array.hpp>

#include <algorithm>
#include <array>
#include <type_traits>
#include <typeinfo>

struct Tuple;

namespace detail {
    template <typename T, T ...vs>
    struct tuple_c { struct hana { using datatype = Tuple; }; };

    template <typename ...ts>
    struct tuple_t {
        struct hidden : tuple_t { };
        struct hana { using datatype = Tuple; };
    };

    template <typename Storage>
    struct tuple {
        Storage storage;
        struct hana { using datatype = Tuple; };
    };
}

template <typename T, T ...vs>
constexpr detail::tuple_c<T, vs...> tuple_c{};

template <typename ...ts>
constexpr typename detail::tuple_t<ts...>::hidden tuple_t{};

auto tuple = [](auto ...xs) {
    auto storage = [=](auto f) { return f(xs...); };
    return detail::tuple<decltype(storage)>{storage};
};

template <typename T, T v>
constexpr auto constant = boost::hana::integral<T, v>;



template <typename F, typename T, T ...vs, std::size_t ...index>
constexpr auto transform_tuple_c_impl(detail::tuple_c<T, vs...> xs, std::index_sequence<index...>) {
    constexpr boost::hana::sandbox::array<T, sizeof...(vs)> omg{{vs...}};
    constexpr auto result = F{}(omg);
    return tuple_c<T, result[index]...>;
}

template <typename F, typename T, T ...vs>
constexpr auto transform_tuple_c(detail::tuple_c<T, vs...> xs) {
    return transform_tuple_c_impl<F>(xs, std::make_index_sequence<sizeof...(vs)>{});
}


namespace boost { namespace hana {
    template <>
    struct unpack_impl< ::Tuple> {
        template <typename Storage, typename F>
        static constexpr auto apply(::detail::tuple<Storage> xs, F f)
        { return xs.storage(f); }

        template <typename T, T ...vs, typename F>
        static constexpr auto apply(::detail::tuple_c<T, vs...>, F f)
        { return f(::constant<T, vs>...); }

        template <typename ...ts, typename F>
        static constexpr auto apply(::detail::tuple_t<ts...>, F f)
        { return f(type<ts>...); }
    };

    template <>
    struct head_impl< ::Tuple> {
        template <typename Storage>
        static constexpr auto apply(::detail::tuple<Storage> xs)
        { return xs.storage([](auto x, auto ...xs) { return x; }); }

        template <typename T, T v, T ...vs>
        static constexpr auto apply(::detail::tuple_c<T, v, vs...>)
        { return ::constant<T, v>; }

        template <typename t, typename ...ts>
        static constexpr auto apply(::detail::tuple_t<t, ts...>)
        { return type<t>; }
    };

    template <>
    struct tail_impl< ::Tuple> {
        template <typename Storage>
        static constexpr auto apply(::detail::tuple<Storage> xs)
        { return xs.storage([](auto x, auto ...xs) { return ::tuple(xs...); }); }

        template <typename T, T v, T ...vs>
        static constexpr auto apply(::detail::tuple_c<T, v, vs...>)
        { return tuple_c<T, vs...>; }

        template <typename t, typename ...ts>
        static constexpr auto apply(::detail::tuple_t<t, ts...>)
        { return tuple_t<ts...>; }
    };

    template <>
    struct is_empty_impl< ::Tuple> {
        template <typename Storage>
        static constexpr auto apply(::detail::tuple<Storage> xs)
        { return xs.storage([](auto ...xs) { return bool_<sizeof...(xs) == 0>; }); }

        template <typename T, T ...vs>
        static constexpr auto apply(::detail::tuple_c<T, vs...>)
        { return bool_<sizeof...(vs) == 0>; }

        template <typename ...ts>
        static constexpr auto apply(::detail::tuple_t<ts...>)
        { return bool_<sizeof...(ts) == 0>; }
    };

    template <>
    struct List::instance< ::Tuple> : List::mcd< ::Tuple> {
        // nil
        static BOOST_HANA_CONSTEXPR_LAMBDA auto nil_impl()
        { return ::tuple(); }

        // cons
        template <typename X, typename Xs, typename Datatype>
        static constexpr auto cons_helper(X x, Xs xs, Datatype)
        { return unpack(xs, [=](auto ...xs) { return ::tuple(x, xs...); }); }


        template <typename X, typename T>
        static constexpr auto cons_helper(X x, decltype(::tuple()) const&, Integral<T>)
        { return tuple_c<decltype(value(x)), value(x)>; }

        template <typename X, typename T, T ...vs, typename U>
        static constexpr auto cons_helper(X x, ::detail::tuple_c<T, vs...> xs, Integral<U>)
        { return tuple_c<T, value(x), vs...>; }


        template <typename X>
        static constexpr auto cons_helper(X x, decltype(::tuple()) const&, Type)
        { return tuple_t<typename X::type>; }

        template <typename X, typename ...ts>
        static constexpr auto cons_helper(X x, ::detail::tuple_t<ts...> xs, Type)
        { return tuple_t<typename X::type, ts...>; }


        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return cons_helper(x, xs, datatype_t<X>{}); }


        // sort
        template <typename Xs>
        static constexpr auto sort_impl(Xs xs)
        { return List::mcd< ::Tuple>::sort_impl(xs); }

        struct insertion_sort {
            template <typename T, std::size_t N>
            constexpr auto operator()(sandbox::array<T, N> a) const {
                for (std::size_t i = 1; i < N; ++i) {
                    std::size_t j = i;
                    while (j > 0 && a[j-1] > a[j]) {
                        auto tmp = a[j];
                        a[j] = a[j-1];
                        a[j-1] = tmp;

                        --j;
                    }
                }
                return a;
            }
        };

        template <typename T, T ...vs>
        static constexpr auto sort_impl(::detail::tuple_c<T, vs...> xs) {
            return transform_tuple_c<insertion_sort>(xs);
        }
    };
}}

using namespace boost::hana;

int main() {
    foldl(tuple_t<int, char, float>, type<int>, [](auto t, auto u) {
        return type<std::common_type_t<
            typename decltype(t)::type,
            typename decltype(u)::type
        >>;
    });

    fmap(tuple_t<int, char, float>, [](auto t) {
        return typeid(t).name();
    });

    fmap(tuple_c<int, 1, 2, 3>, [](auto i) {
        return std::array<int, value(i)>{};
    });

    fmap(tuple_c<int, 1, 2, 3>, [](auto i) {
        return i + int_<4>;
    });
}
