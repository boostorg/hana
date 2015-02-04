/*!
@file
Defines the `boost::hana::Tuple` data type.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_HPP
#define BOOST_HANA_TUPLE_HPP

#include <boost/hana/fwd/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/generate_integer_sequence.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/type.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // tuple, tuple_t and tuple_c
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Xs>
    struct _tuple
        : detail::closure<Xs...>
        , operators::enable_adl
        , operators::Iterable_ops<_tuple<Xs...>>
    {
        using detail::closure<Xs...>::closure; // inherit constructor

        static constexpr detail::std::size_t size = sizeof...(Xs);
        static constexpr bool is_empty = sizeof...(Xs) == 0;
    };

    template <typename ...Xs>
    struct datatype<_tuple<Xs...>> {
        using type = Tuple;
    };

    template <typename ...T>
    struct _tuple_t {
        struct _
            // : _tuple_t
            // , operators::enable_adl
            // , operators::Iterable_ops<_>

            : _tuple<decltype(type<T>...)...>
        {
            // struct hana { using datatype = Tuple; };
            // static constexpr detail::std::size_t size = sizeof...(T);
            // static constexpr bool is_empty = sizeof...(T) == 0;
        };
    };

    template <typename T, T ...v>
    struct _tuple_c
        // : operators::enable_adl
        // , operators::Iterable_ops<_tuple_c<T, v...>>

        : _tuple<decltype(integral_constant<T, v>)...>
    {
        // static constexpr detail::std::size_t size = sizeof...(v);
        // static constexpr bool is_empty = sizeof...(v) == 0;
    };

    template <typename T, T ...v>
    struct datatype<_tuple_c<T, v...>> {
        using type = Tuple;
    };

    template <>
    struct enabled_operators<Tuple>
        : Comparable, Monad, Iterable
    { };

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Tuple> {
        template <typename ...Xs>
        static constexpr
        _tuple<typename detail::std::decay<Xs>::type...> apply(Xs&& ...xs)
        { return {detail::std::forward<Xs>(xs)...}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // utilities
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename F>
        using is_Metafunction = detail::std::is_same<
            typename datatype<F>::type, Metafunction
        >;

        template <typename T>
        using is_Type = detail::std::is_same<
            typename datatype<T>::type, Type
        >;

        template <typename ...Xs>
        constexpr detail::std::size_t tuple_size_impl(_tuple<Xs...>*)
        { return sizeof...(Xs); }

        template <typename ...T>
        constexpr detail::std::size_t tuple_size_impl(_tuple_t<T...>*)
        { return sizeof...(T); }

        template <typename ...T>
        constexpr detail::std::size_t tuple_size_impl(_tuple_c<T...>*)
        { return sizeof...(T); }

        template <typename Xs>
        using tuple_size = detail::std::integral_constant<detail::std::size_t,
            tuple_size_impl((Xs*)0)
        >;

        template <detail::std::size_t n, typename Xn>
        static constexpr decltype(auto)
        tuple_get(detail::element<n, Xn> const& x)
        { return (x.get); }

        template <detail::std::size_t n, typename Xn>
        static constexpr decltype(auto)
        tuple_get(detail::element<n, Xn>& x)
        { return (x.get); }

        template <detail::std::size_t n, typename Xn>
        static constexpr Xn
        tuple_get(detail::element<n, Xn>&& x)
        { return detail::std::move(x.get); }

        template <bool, typename T>
        using expand = T;
    } // end namespace detail

    #define BOOST_HANA_PP_FOR_EACH_REF1(MACRO)                          \
        MACRO(const&)                                                   \
        MACRO(&)                                                        \
        MACRO(&&)                                                       \
    /**/

    #define BOOST_HANA_PP_FOR_EACH_REF2(MACRO)                          \
        MACRO(const&, const&)                                           \
        MACRO(const&, &&    )                                           \
        MACRO(&&    , const&)                                           \
        MACRO(&&    , &&    )                                           \
    /**/

    #define BOOST_HANA_PP_FOR_EACH_REF3(MACRO)                          \
        MACRO(const&, const&, const&)                                   \
        MACRO(const&, const&, &&    )                                   \
        MACRO(const&, &&    , const&)                                   \
        MACRO(const&, &&    , &&    )                                   \
        MACRO(&&    , const&, const&)                                   \
        MACRO(&&    , const&, &&    )                                   \
        MACRO(&&    , &&    , const&)                                   \
        MACRO(&&    , &&    , &&    )                                   \
    /**/

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct equal_impl<Tuple, Tuple> {
        template <typename Xs, typename Ys>
        static constexpr decltype(auto) apply(Xs const& xs, Ys const& ys)
        { return Sequence::equal_impl<Tuple, Tuple>::apply(xs, ys); }


        // tuple_t optimizations
        template <typename ...T>
        static constexpr auto apply(_tuple_t<T...>, _tuple_t<T...>)
        { return true_; }

        template <typename ...T, typename ...U>
        static constexpr auto apply(_tuple_t<T...>, _tuple_t<U...>)
        { return false_; }


        // tuple_c optimizations
        template <typename V, V ...v, typename U, U ...u, typename =
            detail::std::enable_if_t<sizeof...(v) == sizeof...(u)>>
        static constexpr auto apply(_tuple_c<V, v...>, _tuple_c<U, u...>) {
            constexpr bool comparisons[] = {(v == u)...};
            return bool_<hana::all_of(comparisons)>;
        }

        template <typename V, V ...v, typename U, U ...u, typename =
            detail::std::enable_if_t<sizeof...(v) != sizeof...(u)>>
        static constexpr auto apply(_tuple_c<V, v...>, _tuple_c<U, u...>)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct unpack_impl<Tuple> {
        #define BOOST_HANA_PP_UNPACK(REF)                                   \
            template <typename ...Xs, typename F>                           \
            static constexpr decltype(auto)                                 \
            apply(detail::closure_impl<Xs...> REF xs, F&& f) {              \
                return detail::std::forward<F>(f)(                          \
                    static_cast<Xs REF>(xs).get...                          \
                );                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_UNPACK)
        #undef BOOST_HANA_PP_UNPACK


        template <typename ...T, typename F>
        static constexpr auto helper(F const&, Metafunction*)
        { return type<typename F::template apply<T...>::type>; }

        template <typename ...T, typename F>
        static constexpr decltype(auto) helper(F&& f, ...)
        { return detail::std::forward<F>(f)(type<T>...); }

        template <typename ...T, typename F>
        static constexpr decltype(auto) apply(_tuple_t<T...> const&, F&& f) {
            return helper<T...>(detail::std::forward<F>(f),
                                (typename datatype<F>::type*)0);
        }
    };

    template <>
    struct length_impl<Tuple> {
        template <typename Xs>
        static constexpr auto apply(Xs const&)
        { return size_t<Xs::size>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Iterable(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct head_impl<Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return detail::tuple_get<0>(detail::std::forward<Xs>(xs)); }
    };

    template <>
    struct at_impl<Tuple> {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N, Xs&& xs) {
            constexpr detail::std::size_t index = hana::value2<N>();
            return detail::tuple_get<index>(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct tail_impl<Tuple> {
        #define BOOST_HANA_PP_TAIL(REF)                                     \
            template <typename X, typename ...Xn>                           \
            static constexpr _tuple<typename Xn::get_type...>               \
            apply(detail::closure_impl<X, Xn...> REF xs) {                  \
                return {static_cast<Xn REF>(xs).get...};                    \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_TAIL)
        #undef BOOST_HANA_PP_TAIL

        template <typename T, typename ...Ts>
        static constexpr auto apply(_tuple_t<T, Ts...> const&)
        { return tuple_t<Ts...>; }
    };

    template <>
    struct is_empty_impl<Tuple> {
        template <typename Xs>
        static constexpr auto apply(Xs const&)
        { return bool_<Xs::is_empty>; }
    };

    template <>
    struct last_impl<Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            constexpr detail::std::size_t size = detail::tuple_size<Xs>{};
            return detail::tuple_get<size - 1>(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct drop_impl<Tuple> {
        using size_t = detail::std::size_t;

        template <size_t n, typename Xs, size_t ...i>
        static constexpr decltype(auto)
        helper(Xs&& xs, detail::std::index_sequence<i...>) {
            return hana::tuple(detail::tuple_get<n + i>(
                                            detail::std::forward<Xs>(xs))...);
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N, Xs&& xs) {
            constexpr size_t n = hana::value2<N>();
            constexpr size_t size = detail::tuple_size<Xs>{};
            constexpr size_t drop_size = n > size ? size : n;
            return helper<drop_size>(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<size - drop_size>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Searchable(Tuple)>
        : detail::std::true_type
    { };

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Traversable(Tuple)>
        : detail::std::true_type
    { };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Functor(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct transform_impl<Tuple> {
        #define BOOST_HANA_PP_TRANSFORM(REF)                                \
            template <typename ...Xs, typename F>                           \
            static constexpr decltype(auto)                                 \
            apply(detail::closure_impl<Xs...> REF xs, F&& f)                \
            { return hana::tuple(f(static_cast<Xs REF>(xs).get)...); }      \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_TRANSFORM)
        #undef BOOST_HANA_PP_TRANSFORM


        template <typename ...T, typename F, typename = detail::std::enable_if_t<
            !detail::is_metafunction<F>{}
        >>
        static constexpr decltype(auto) apply(_tuple_t<T...> const&, F&& f) {
            return hana::tuple(f(type<T>)...);
        }

        template <typename ...T, typename F, typename = detail::std::enable_if_t<
            detail::is_metafunction<F>{}
        >>
        static constexpr decltype(auto) apply(_tuple_t<T...> const&, F const&) {
            return tuple_t<typename F::template apply<T>::type...>;
        }
    };

    template <>
    struct fill_impl<Tuple> {
        template <typename ...Xs, typename T>
        static constexpr auto apply(_tuple_t<Xs...> const&, _type<T> const&)
        { return tuple_t<detail::expand<sizeof(Xs), T>...>; }

        template <typename V>
        static constexpr auto apply(_tuple_t<> const&, V&&)
        { return tuple_t<>; }

        template <typename T, typename ...Ts, typename V>
        static constexpr _tuple<
            typename detail::std::decay<V>::type,
            typename detail::std::decay<detail::expand<sizeof(Ts), V>>::type...
        > apply(_tuple_t<T, Ts...> const&, V&& v)
        { return {((void)sizeof(Ts), v)..., detail::std::forward<V>(v)}; }

        template <typename ...Xs, typename V>
        static constexpr decltype(auto) apply(_tuple<Xs...> const&, V&& v)
        { return apply(tuple_t<Xs...>, detail::std::forward<V>(v)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Applicative(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct lift_impl<Tuple> {
        template <typename X>
        static constexpr _tuple<typename detail::std::decay<X>::type> apply(X&& x)
        { return {detail::std::forward<X>(x)}; }
    };

    template <>
    struct ap_impl<Tuple> {

    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Monad(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct flatten_impl<Tuple> {
        using size_t = detail::std::size_t;

        template <int Which, size_t ...Lengths>
        struct flatten_indices {
            template <typename Array>
            constexpr auto operator()(Array outer) const {
                constexpr size_t lengths[] = {Lengths...};
                Array inner = outer;
                for (size_t index = 0, i = 0; i < sizeof...(Lengths); ++i) {
                    for (size_t j = 0; j < lengths[i]; ++j, ++index) {
                        inner[index] = i;
                        outer[index] = j;
                    }
                }
                return detail::tuple_get<Which>(hana::tuple(outer, inner));
            }
        };

        template <typename Xs, size_t ...outer, size_t ...inner>
        static constexpr decltype(auto)
        helper(Xs&& xs, detail::std::index_sequence<outer...>,
                        detail::std::index_sequence<inner...>)
        {
            return hana::tuple(detail::tuple_get<outer>(
                                detail::tuple_get<inner>(
                                    detail::std::forward<Xs>(xs)))...);
        }

        #define BOOST_HANA_PP_FLATTEN(REF)                                      \
            template <typename ...Xs>                                           \
            static constexpr decltype(auto)                                     \
            apply(detail::closure_impl<Xs...> REF xs) {                         \
                constexpr size_t lengths[] = {detail::tuple_size<Xs>{}...};     \
                constexpr size_t total_length = hana::sum(lengths);             \
                                                                                \
                using Outer = flatten_indices<0, detail::tuple_size<Xs>{}...>;  \
                using Inner = flatten_indices<1, detail::tuple_size<Xs>{}...>;  \
                                                                                \
                return helper(static_cast<detail::closure_impl<Xs...> REF>(xs), \
                    detail::generate_index_sequence<total_length, Outer>{},     \
                    detail::generate_index_sequence<total_length, Inner>{});    \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_FLATTEN)
        #undef BOOST_HANA_PP_FLATTEN
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<MonadPlus(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct concat_impl<Tuple> {
        #define BOOST_HANA_PP_CONCAT(REF1, REF2)                            \
            template <typename ...Xs, typename ...Ys>                       \
            static constexpr _tuple<                                        \
                typename Xs::get_type..., typename Ys::get_type...          \
            > apply(detail::closure_impl<Xs...> REF1 xs,                    \
                    detail::closure_impl<Ys...> REF2 ys)                    \
            {                                                               \
                return {                                                    \
                    static_cast<Xs REF1>(xs).get...,                        \
                    static_cast<Ys REF2>(ys).get...                         \
                };                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF2(BOOST_HANA_PP_CONCAT)
        #undef BOOST_HANA_PP_CONCAT

        #define BOOST_HANA_PP_CONCAT_T(REF)                                 \
            template <typename ...Xs, typename ...Us>                       \
            static constexpr _tuple<                                        \
                typename Xs::get_type..., decltype(type<Us>)...             \
            > apply(detail::closure_impl<Xs...> REF xs,                     \
                    _tuple_t<Us...> const&)                                 \
            { return {static_cast<Xs REF>(xs).get..., type<Us>...}; }       \
                                                                            \
            template <typename ...Ts, typename ...Ys>                       \
            static constexpr _tuple<                                        \
                decltype(type<Ts>)..., typename Ys::get_type...             \
            > apply(_tuple_t<Ts...> const&,                                 \
                    detail::closure_impl<Ys...> REF ys)                     \
            { return {type<Ts>..., static_cast<Ys REF>(ys).get...}; }       \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_CONCAT_T)
        #undef BOOST_HANA_PP_CONCAT_T

        template <typename ...Ts, typename ...Us>
        static constexpr auto apply(_tuple_t<Ts...> const&, _tuple_t<Us...> const&)
        { return tuple_t<Ts..., Us...>; }
    };

    template <>
    struct nil_impl<Tuple> {
        static constexpr auto apply()
        { return tuple_t<>; }
    };

    template <>
    struct cycle_impl<Tuple> {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N n, Xs&& xs)
        { return hana::flatten(hana::repeat(n, detail::std::forward<Xs>(xs))); }
    };

    template <>
    struct repeat_impl<Tuple> {
        template <typename X>
        static constexpr auto helper(X&&, detail::std::index_sequence<>, ...)
        { return tuple_t<>; }

        template <typename X, detail::std::size_t i, detail::std::size_t ...j>
        static constexpr _tuple<
            typename detail::std::decay<X>::type,
            typename detail::std::decay<detail::expand<j, X>>::type...
        > helper(X&& x, detail::std::index_sequence<i, j...>, ...)
        { return {((void)j, x)..., detail::std::forward<X>(x)}; }

        template <typename T, detail::std::size_t i, detail::std::size_t ...j>
        static constexpr auto helper(T, detail::std::index_sequence<i, j...>, Type*)
        { return tuple_t<typename T::type, detail::expand<j, typename T::type>...>; }

        template <typename N, typename X>
        static constexpr decltype(auto) apply(N, X&& x) {
            constexpr detail::std::size_t n = hana::value2<N>();
            return helper(detail::std::forward<X>(x),
                          detail::std::make_index_sequence<n>{},
                          (typename datatype<X>::type*)0);
        }
    };

    template <>
    struct prepend_impl<Tuple> {
        #define BOOST_HANA_PP_PREPEND(REF)                                      \
            template <typename X, typename ...Xs>                               \
            static constexpr _tuple<                                            \
                typename detail::std::decay<X>::type, typename Xs::get_type...  \
            > apply(X&& x, detail::closure_impl<Xs...> REF xs) {                \
                return {                                                        \
                    detail::std::forward<X>(x), static_cast<Xs REF>(xs).get...  \
                };                                                              \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_PREPEND)
        #undef BOOST_HANA_PP_PREPEND
    };

    template <>
    struct append_impl<Tuple> {
        #define BOOST_HANA_PP_APPEND(REF)                                       \
            template <typename ...Xs, typename X>                               \
            static constexpr _tuple<                                            \
                typename Xs::get_type..., typename detail::std::decay<X>::type  \
            > apply(detail::closure_impl<Xs...> REF xs, X&& x) {                \
                return {                                                        \
                    static_cast<Xs REF>(xs).get..., detail::std::forward<X>(x)  \
                };                                                              \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_APPEND)
        #undef BOOST_HANA_PP_APPEND
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Sequence(Tuple)>
        : detail::std::true_type
    { };

    template <>
    struct init_impl<Tuple> {
        template <typename Xs, detail::std::size_t ...n>
        static constexpr decltype(auto)
        helper(Xs&& xs, detail::std::index_sequence<n...>) {
            return hana::tuple(detail::tuple_get<n>(
                    detail::std::forward<Xs>(xs))...);
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            constexpr detail::std::size_t size = detail::tuple_size<Xs>{};
            return helper(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<size - 1>{});
        }
    };

    template <>
    struct intersperse_impl<Tuple> {
        template <detail::std::size_t i, typename Z, typename Xs>
        static constexpr Z pick(Z&& z, Xs const&,
            detail::std::false_type /* odd index */)
        { return detail::std::forward<Z>(z); }

        template <detail::std::size_t i, typename Z, typename Xn>
        static constexpr decltype(auto)
        pick(Z const&, detail::element<(i + 1) / 2, Xn> const& x,
            detail::std::true_type /* even index */)
        { return x.get; }

        template <detail::std::size_t i, typename Z, typename Xn>
        static constexpr decltype(auto)
        pick(Z const&, detail::element<(i + 1) / 2, Xn>&& x,
            detail::std::true_type /* even index */)
        { return detail::std::move(x.get); }

        template <typename Xs, typename Z, detail::std::size_t ...i>
        static constexpr decltype(auto)
        helper(Xs&& xs, Z&& z, detail::std::index_sequence<i...>) {
            return hana::tuple(
                pick<i>(
                    detail::std::forward<Z>(z),
                    detail::std::forward<Xs>(xs),
                    detail::std::integral_constant<bool, (i % 2 == 0)>{}
                )...
            );
        }

        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            constexpr detail::std::size_t size = detail::tuple_size<Xs>{};
            constexpr auto new_seq_size = size == 0 ? 0 : (size * 2) - 1;
            return helper(detail::std::forward<Xs>(xs),
                          detail::std::forward<Z>(z),
                          detail::std::make_index_sequence<new_seq_size>{});
        }
    };

    template <>
    struct remove_at_impl<Tuple> {
        using size_t = detail::std::size_t;

        template <typename Xs, size_t ...before, size_t ...after>
        static constexpr decltype(auto)
        helper(Xs&& xs, detail::std::index_sequence<before...>,
                        detail::std::index_sequence<after...>)
        {
            return hana::tuple(
                detail::tuple_get<before>(
                    detail::std::forward<Xs>(xs)
                )...,
                detail::tuple_get<sizeof...(before) + after + 1>(
                    detail::std::forward<Xs>(xs)
                )...
            );
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N, Xs&& xs) {
            constexpr size_t index = hana::value2<N>();
            constexpr size_t size = detail::tuple_size<Xs>{};
            return helper(detail::std::forward<Xs>(xs),
                          detail::std::make_index_sequence<index>{},
                          detail::std::make_index_sequence<size - index - 1>{});
        }
    };

    template <>
    struct reverse_impl<Tuple> {
        template <detail::std::size_t ...n, typename ...Xn>
        static constexpr decltype(auto)
        apply(detail::closure_impl<detail::element<n, Xn>...>&& xs) {
            return hana::tuple(detail::std::move(
                    detail::tuple_get<sizeof...(n) - n - 1>(xs))...);
        }

        template <detail::std::size_t ...n, typename ...Xn>
        static constexpr decltype(auto)
        apply(detail::closure_impl<detail::element<n, Xn>...> const& xs) {
            return hana::tuple(detail::tuple_get<sizeof...(n) - n - 1>(xs)...);
        }
    };

    template <>
    struct slice_impl<Tuple> {
        template <detail::std::size_t from, typename Xs, detail::std::size_t ...i>
        static constexpr decltype(auto)
        helper(Xs&& xs, detail::std::index_sequence<i...>) {
            return hana::tuple(detail::tuple_get<from + i>(
                                    detail::std::forward<Xs>(xs))...);
        }

        template <typename Xs, typename From, typename To>
        static constexpr decltype(auto) apply(Xs&& xs, From, To) {
            constexpr detail::std::size_t from = hana::value2<From>(),
                                          to = hana::value2<To>();
            return helper<from>(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<to - from>{}
            );
        }
    };

    template <>
    struct take_impl<Tuple> {
        template <typename Xs, detail::std::size_t ...n>
        static constexpr decltype(auto)
        helper(Xs&& xs, detail::std::index_sequence<n...>) {
            return hana::tuple(detail::tuple_get<n>(
                                        detail::std::forward<Xs>(xs))...);
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N, Xs&& xs) {
            constexpr detail::std::size_t n = hana::value2<N>(),
                                          size = detail::tuple_size<Xs>{};
            return helper(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<(n < size ? n : size)>{});
        }
    };

    template <>
    struct unzip_impl<Tuple> {
        #define BOOST_HANA_PP_UNZIP(REF)                                    \
            template <typename ...Xs>                                       \
            static constexpr decltype(auto)                                 \
            apply(detail::closure_impl<Xs...> REF xs)                       \
            { return hana::zip(static_cast<Xs REF>(xs).get...); }           \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_UNZIP)
        #undef BOOST_HANA_PP_UNZIP
    };

    template <>
    struct zip_with_impl<Tuple> {
        #define BOOST_HANA_PP_ZIP_WITH1(REF)                                \
            template <typename F, typename ...Xs>                           \
            static constexpr decltype(auto) zip_with_helper(F&& f,          \
                detail::closure_impl<Xs...> REF xs)                         \
            { return hana::tuple(f(static_cast<Xs REF>(xs).get)...); }      \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_ZIP_WITH1)
        #undef BOOST_HANA_PP_ZIP_WITH1

        #define BOOST_HANA_PP_ZIP_WITH2(REF1, REF2)                         \
            template <typename F, typename ...Xs, typename ...Ys>           \
            static constexpr decltype(auto) zip_with_helper(F&& f,          \
                detail::closure_impl<Xs...> REF1 xs,                        \
                detail::closure_impl<Ys...> REF2 ys)                        \
            {                                                               \
                return hana::tuple(                                         \
                    f(static_cast<Xs REF1>(xs).get,                         \
                      static_cast<Ys REF2>(ys).get)...                      \
                );                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF2(BOOST_HANA_PP_ZIP_WITH2)
        #undef BOOST_HANA_PP_ZIP_WITH2

        #define BOOST_HANA_PP_ZIP_WITH3(REF1, REF2, REF3)                         \
            template <typename F, typename ...Xs, typename ...Ys, typename ...Zs> \
            static constexpr decltype(auto) zip_with_helper(F&& f,                \
                detail::closure_impl<Xs...> REF1 xs,                              \
                detail::closure_impl<Ys...> REF2 ys,                              \
                detail::closure_impl<Zs...> REF3 zs)                              \
            {                                                                     \
                return hana::tuple(                                               \
                    f(static_cast<Xs REF1>(xs).get,                               \
                      static_cast<Ys REF2>(ys).get,                               \
                      static_cast<Zs REF3>(zs).get)...                            \
                );                                                                \
            }                                                                     \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF3(BOOST_HANA_PP_ZIP_WITH3)
        #undef BOOST_HANA_PP_ZIP_WITH3

        template <typename F, typename X1, typename X2, typename X3, typename X4, typename ...Xn>
        static constexpr decltype(auto)
        zip_with_helper(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xn&& ...xn) {
            return zip_with_helper(hana::apply,
                zip_with_helper(
                    curry<sizeof...(xn) + 4>(detail::std::forward<F>(f)),
                    detail::std::forward<X1>(x1),
                    detail::std::forward<X2>(x2),
                    detail::std::forward<X3>(x3)
                ),
                detail::std::forward<X4>(x4),
                detail::std::forward<Xn>(xn)...
            );
        }

        template <typename F, typename Xs, typename ...Ys>
        static constexpr decltype(auto) apply(F&& f, Xs&& xs, Ys&& ...ys) {
            constexpr detail::std::size_t lengths[] = {
                detail::tuple_size<Xs>{}, detail::tuple_size<Ys>{}...
            };
            constexpr detail::std::size_t zip_length = hana::minimum(lengths);
            return zip_with_helper(detail::std::forward<F>(f),
                take_c<zip_length>(detail::std::forward<Xs>(xs)),
                take_c<zip_length>(detail::std::forward<Ys>(ys))...
            );
        }
    };

    template <>
    struct zip_impl<Tuple> {
        #define BOOST_HANA_PP_ZIP1(REF)                                     \
            template <typename ...Xs>                                       \
            static constexpr _tuple<                                        \
                _tuple<typename Xs::get_type>...                            \
            > zip_helper(detail::closure_impl<Xs...> REF xs) {              \
                return {                                                    \
                    _tuple<typename Xs::get_type>{                          \
                        static_cast<Xs REF>(xs).get                         \
                    }...                                                    \
                };                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_ZIP1)
        #undef BOOST_HANA_PP_ZIP1

        #define BOOST_HANA_PP_ZIP2(REF1, REF2)                              \
            template <typename ...Xs, typename ...Ys>                       \
            static constexpr _tuple<                                        \
                _tuple<typename Xs::get_type,                               \
                       typename Ys::get_type>...                            \
            > zip_helper(detail::closure_impl<Xs...> REF1 xs,               \
                         detail::closure_impl<Ys...> REF2 ys) {             \
                return {                                                    \
                    _tuple<typename Xs::get_type,                           \
                           typename Ys::get_type>{                          \
                        static_cast<Xs REF1>(xs).get,                       \
                        static_cast<Ys REF2>(ys).get                        \
                    }...                                                    \
                };                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF2(BOOST_HANA_PP_ZIP2)
        #undef BOOST_HANA_PP_ZIP2

        #define BOOST_HANA_PP_ZIP3(REF1, REF2, REF3)                    \
            template <typename ...Xs, typename ...Ys, typename ...Zs>   \
            static constexpr _tuple<                                    \
                _tuple<typename Xs::get_type,                           \
                       typename Ys::get_type,                           \
                       typename Zs::get_type>...                        \
            > zip_helper(detail::closure_impl<Xs...> REF1 xs,           \
                         detail::closure_impl<Ys...> REF2 ys,           \
                         detail::closure_impl<Zs...> REF3 zs) {         \
                return {                                                \
                    _tuple<typename Xs::get_type,                       \
                           typename Ys::get_type,                       \
                           typename Zs::get_type>{                      \
                        static_cast<Xs REF1>(xs).get,                   \
                        static_cast<Ys REF2>(ys).get,                   \
                        static_cast<Zs REF3>(zs).get                    \
                    }...                                                \
                };                                                      \
            }                                                           \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF3(BOOST_HANA_PP_ZIP3)
        #undef BOOST_HANA_PP_ZIP3

        template <typename X1, typename X2, typename X3, typename X4, typename ...Xn>
        static constexpr decltype(auto)
        zip_helper(X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xn&& ...xn) {
            return hana::zip_with(concat,
                zip_helper(
                    detail::std::forward<X1>(x1),
                    detail::std::forward<X2>(x2),
                    detail::std::forward<X3>(x3)
                ),
                zip_helper(
                    detail::std::forward<X4>(x4),
                    detail::std::forward<Xn>(xn)...
                )
            );
        }

        template <typename Xs, typename ...Ys>
        static constexpr decltype(auto) apply(Xs&& xs, Ys&& ...ys) {
            constexpr detail::std::size_t lengths[] = {
                detail::tuple_size<Xs>{}, detail::tuple_size<Ys>{}...
            };
            constexpr detail::std::size_t zip_length = hana::minimum(lengths);
            return zip_helper(
                take_c<zip_length>(detail::std::forward<Xs>(xs)),
                take_c<zip_length>(detail::std::forward<Ys>(ys))...
            );
        }
    };

#undef BOOST_HANA_PP_FOR_EACH_REF1
#undef BOOST_HANA_PP_FOR_EACH_REF2
#undef BOOST_HANA_PP_FOR_EACH_REF3
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_HPP
