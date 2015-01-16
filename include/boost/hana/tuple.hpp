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
#include <boost/hana/constant.hpp> // value
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
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/integral.hpp> // size_t

// instances
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>

// operators
#include <boost/hana/comparable.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    namespace tuple_detail {
        struct utils {
            template <typename ...Xs>
            static constexpr detail::std::size_t get_size(_tuple<Xs...> const*)
            { return sizeof...(Xs); }

            template <detail::std::size_t n, typename Xn>
            static constexpr decltype(auto)
            get_element(detail::element<n, Xn> const& x)
            { return (x.get); }

            template <detail::std::size_t n, typename Xn>
            static constexpr decltype(auto)
            get_element(detail::element<n, Xn>& x)
            { return (x.get); }

            template <detail::std::size_t n, typename Xn>
            static constexpr Xn
            get_element(detail::element<n, Xn>&& x)
            { return detail::std::move(x.get); }

            template <bool, typename X>
            using allow_expansion = X;

            template <typename T, detail::std::size_t N>
            static constexpr T homogeneous_min(T const (&array)[N]) {
                static_assert(N > 0, "can't find the minimum of an empty array");
                T m = array[0];
                for (detail::std::size_t i = 0; i < N; ++i)
                    if (array[i] < m)
                        m = array[i];
                return m;
            }

            template <typename T, detail::std::size_t N>
            static constexpr T homogeneous_sum(T const (&array)[N]) {
                T s = 0;
                for (detail::std::size_t i = 0; i < N; ++i)
                    s += array[i];
                return s;
            }
        };
    }

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

    template <>
    struct unpack_impl<Tuple> {
        #define BOOST_HANA_PP_UNPACK(REF)                                   \
            template <typename Into, typename ...Xs>                        \
            static constexpr decltype(auto)                                 \
            apply(detail::closure_impl<Xs...> REF xs, Into&& into) {        \
                return detail::std::forward<Into>(into)(                    \
                    static_cast<Xs REF>(xs).get...                          \
                );                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_UNPACK)
        #undef BOOST_HANA_PP_UNPACK
    };

    template <>
    struct length_impl<Tuple> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(_tuple<Xs...> const&)
        { return size_t<sizeof...(Xs)>; }
    };

    template <>
    struct head_impl<Tuple> : tuple_detail::utils {
        template <typename Tuple>
        static constexpr decltype(auto) apply(Tuple&& tuple)
        { return get_element<0>(detail::std::forward<Tuple>(tuple)); }
    };

    template <>
    struct at_impl<Tuple> : tuple_detail::utils {
        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N n, Xs&& xs) {
            constexpr detail::std::size_t index = value(n);
            return get_element<index>(detail::std::forward<Xs>(xs));
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
    };

    template <>
    struct is_empty_impl<Tuple> {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(_tuple<Xs...> const&)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <>
    struct last_impl<Tuple> : tuple_detail::utils {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            constexpr detail::std::size_t size = get_size(decltype(&xs){});
            return get_element<size - 1>(detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct drop_impl<Tuple> : tuple_detail::utils {
        template <detail::std::size_t n, detail::std::size_t ...i, typename Xs>
        static constexpr decltype(auto)
        drop_helper(detail::std::index_sequence<i...>, Xs&& xs) {
            return tuple(
                get_element<n + i>(detail::std::forward<Xs>(xs))...
            );
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) apply(N n_, Xs&& xs) {
            constexpr detail::std::size_t n = value(n_);
            constexpr detail::std::size_t size = get_size(decltype(&xs){});
            constexpr detail::std::size_t drop_size = n > size ? size : n;
            return drop_helper<drop_size>(
                detail::std::make_index_sequence<size - drop_size>{},
                detail::std::forward<Xs>(xs));
        }
    };

    template <>
    struct make_impl<Tuple> {
        template <typename ...Xs>
        static constexpr
        _tuple<typename detail::std::decay<Xs>::type...> apply(Xs&& ...xs)
        { return {detail::std::forward<Xs>(xs)...}; }
    };

    template <>
    struct List::instance<Tuple>
        : List::mcd<Tuple>
        , tuple_detail::utils
    {
        // concat
        ///////////////
        #define BOOST_HANA_PP_CONCAT2(REF1, REF2)                           \
            template <typename ...Xs, typename ...Ys>                       \
            static constexpr _tuple<                                        \
                typename Xs::get_type..., typename Ys::get_type...          \
            > concat_impl(detail::closure_impl<Xs...> REF1 xs,              \
                          detail::closure_impl<Ys...> REF2 ys)              \
            {                                                               \
                return {                                                    \
                    static_cast<Xs REF1>(xs).get...,                        \
                    static_cast<Ys REF2>(ys).get...                         \
                };                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF2(BOOST_HANA_PP_CONCAT2)
        #undef BOOST_HANA_PP_CONCAT2

        #define BOOST_HANA_PP_CONCAT3(REF1, REF2, REF3)                     \
            template <typename ...Xs, typename ...Ys, typename ...Zs>       \
            static constexpr _tuple<                                        \
                typename Xs::get_type...,                                   \
                typename Ys::get_type...,                                   \
                typename Zs::get_type...                                    \
            > concat_impl(detail::closure_impl<Xs...> REF1 xs,              \
                          detail::closure_impl<Ys...> REF2 ys,              \
                          detail::closure_impl<Zs...> REF3 zs)              \
            {                                                               \
                return {                                                    \
                    static_cast<Xs REF1>(xs).get...,                        \
                    static_cast<Ys REF2>(ys).get...,                        \
                    static_cast<Zs REF2>(zs).get...                         \
                };                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF3(BOOST_HANA_PP_CONCAT3)
        #undef BOOST_HANA_PP_CONCAT3

        template <int Which, detail::std::size_t ...Lengths>
        struct generate_concat_indices {
            template <typename Array>
            constexpr auto operator()(Array outer) const {
                constexpr detail::std::size_t lengths[] = {Lengths...};
                Array inner = outer;
                for (detail::std::size_t index = 0, i = 0; i < sizeof...(Lengths); ++i) {
                    for (detail::std::size_t j = 0; j < lengths[i]; ++j, ++index) {
                        inner[index] = i;
                        outer[index] = j;
                    }
                }
                return get_element<Which>(tuple(outer, inner));
            }
        };

        template <typename Tuples, detail::std::size_t ...outer, detail::std::size_t ...inner>
        static constexpr decltype(auto) concat_helper(Tuples&& tuples, detail::std::index_sequence<outer...>, detail::std::index_sequence<inner...>) {
            return tuple(get_element<outer>(get_element<inner>(detail::std::forward<Tuples>(tuples)))...);
        }

        // The other overloads are more efficient and we prefer them when there
        // are only 2 or 3 tuples to concatenate.
        template <typename ...Tuples, typename = detail::std::enable_if_t<(sizeof...(Tuples) > 3)>>
        static constexpr decltype(auto) concat_impl(Tuples&& ...tuples) {
            constexpr detail::std::size_t lengths[] = {get_size(decltype(&tuples){})...};
            constexpr detail::std::size_t total_length = homogeneous_sum(lengths);
            using Outer = generate_concat_indices<0, get_size(decltype(&tuples){})...>;
            using Inner = generate_concat_indices<1, get_size(decltype(&tuples){})...>;
            return concat_helper(tuple(detail::std::forward<Tuples>(tuples)...),
                detail::generate_index_sequence<total_length, Outer>{},
                detail::generate_index_sequence<total_length, Inner>{});
        }


        // cons
        ///////////////
        #define BOOST_HANA_PP_CONS(REF)                                         \
            template <typename X, typename ...Xs>                               \
            static constexpr _tuple<                                            \
                typename detail::std::decay<X>::type, typename Xs::get_type...  \
            > cons_impl(X&& x, detail::closure_impl<Xs...> REF xs) {            \
                return {                                                        \
                    detail::std::forward<X>(x), static_cast<Xs REF>(xs).get...  \
                };                                                              \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_CONS)
        #undef BOOST_HANA_PP_CONS


        // init
        ///////////////
        template <typename Xs, detail::std::size_t ...n>
        static constexpr decltype(auto)
        init_helper(Xs&& xs, detail::std::index_sequence<n...>) {
            return tuple(get_element<n>(detail::std::forward<Xs>(xs))...);
        }

        template <typename Xs>
        static constexpr decltype(auto) init_impl(Xs&& xs) {
            constexpr detail::std::size_t size = get_size(decltype(&xs){});
            return init_helper(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<size - 1>{});
        }


        // intersperse
        ///////////////
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
        intersperse_helper(Xs&& xs, Z&& z, detail::std::index_sequence<i...>) {
            return tuple(
                pick<i>(
                    detail::std::forward<Z>(z),
                    detail::std::forward<Xs>(xs),
                    detail::std::integral_constant<bool, (i % 2 == 0)>{}
                )...
            );
        }

        template <typename Xs, typename Z>
        static constexpr decltype(auto) intersperse_impl(Xs&& xs, Z&& z) {
            constexpr detail::std::size_t size = get_size(decltype(&xs){});
            constexpr auto new_seq_size = size == 0 ? 0 : (size * 2) - 1;
            return intersperse_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Z>(z),
                detail::std::make_index_sequence<new_seq_size>{}
            );
        }


        // nil
        ///////////////
        static constexpr _tuple<> nil_impl() { return {}; }


        // remove_at
        ///////////////
        template <typename Xs, detail::std::size_t ...before,
                               detail::std::size_t ...after>
        static constexpr decltype(auto)
        remove_at_helper(Xs&& xs, detail::std::index_sequence<before...>,
                                  detail::std::index_sequence<after...>) {
            return tuple(
                get_element<before>(detail::std::forward<Xs>(xs))...,
                get_element<sizeof...(before) + after + 1>(
                    detail::std::forward<Xs>(xs)
                )...
            );
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) remove_at_impl(N n, Xs&& xs) {
            constexpr detail::std::size_t index = value(n);
            constexpr detail::std::size_t size = get_size(decltype(&xs){});
            return remove_at_helper(
                detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<index>{},
                detail::std::make_index_sequence<size - index - 1>{}
            );
        }


        // repeat
        ///////////////
        template <typename X>
        static constexpr _tuple<>
        repeat_helper(X&& x, detail::std::index_sequence<>) { return {}; }

        template <typename X, detail::std::size_t i, detail::std::size_t ...j>
        static constexpr _tuple<
            typename detail::std::decay<X>::type,
            allow_expansion<(j, false), typename detail::std::decay<X>::type>...
        >
        repeat_helper(X&& x, detail::std::index_sequence<i, j...>) {
            return {((void)j, x)..., detail::std::forward<X>(x)};
        }

        template <typename N, typename X>
        static constexpr decltype(auto) repeat_impl(N n_, X&& x) {
            constexpr detail::std::size_t n = value(n_);
            return repeat_helper(detail::std::forward<X>(x),
                detail::std::make_index_sequence<n>{}
            );
        }


        // reverse
        ///////////////
        template <detail::std::size_t ...n, typename ...Xn>
        static constexpr decltype(auto)
        reverse_impl(detail::closure_impl<detail::element<n, Xn>...>&& xs) {
            return tuple(
                detail::std::move(get_element<sizeof...(n) - n - 1>(xs))...
            );
        }

        template <detail::std::size_t ...n, typename ...Xn>
        static constexpr decltype(auto)
        reverse_impl(detail::closure_impl<detail::element<n, Xn>...> const& xs) {
            return tuple(get_element<sizeof...(n) - n - 1>(xs)...);
        }

        // slice
        ///////////////
        template <detail::std::size_t from, typename Xs, detail::std::size_t ...i>
        static constexpr decltype(auto)
        slice_helper(Xs&& xs, detail::std::index_sequence<i...>) {
            return tuple(get_element<from + i>(detail::std::forward<Xs>(xs))...);
        }

        template <typename Xs, typename From, typename To>
        static constexpr decltype(auto) slice_impl(Xs&& xs, From from_, To to_) {
            constexpr detail::std::size_t from = value(from_), to = value(to_);
            return slice_helper<from>(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<to - from>{}
            );
        }

        // snoc
        ///////////////
        #define BOOST_HANA_PP_SNOC(REF)                                         \
            template <typename ...Xs, typename X>                               \
            static constexpr _tuple<                                            \
                typename Xs::get_type..., typename detail::std::decay<X>::type  \
            > snoc_impl(detail::closure_impl<Xs...> REF xs, X&& x) {            \
                return {                                                        \
                    static_cast<Xs REF>(xs).get...,                             \
                    detail::std::forward<X>(x)                                  \
                };                                                              \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_SNOC)
        #undef BOOST_HANA_PP_SNOC


        // take
        ///////////////
        template <typename Xs, detail::std::size_t ...n>
        static constexpr decltype(auto)
        take_helper(Xs&& xs, detail::std::index_sequence<n...>) {
            return tuple(get_element<n>(detail::std::forward<Xs>(xs))...);
        }

        template <typename N, typename Xs>
        static constexpr decltype(auto) take_impl(N n_, Xs&& xs) {
            constexpr detail::std::size_t n = value(n_);
            constexpr detail::std::size_t size = get_size(decltype(&xs){});
            return take_helper(detail::std::forward<Xs>(xs),
                detail::std::make_index_sequence<(n < size ? n : size)>{});
        }


        // unzip
        ///////////////
        #define BOOST_HANA_PP_UNZIP(REF)                                    \
            template <typename ...Xs>                                       \
            static constexpr decltype(auto)                                 \
            unzip_impl(detail::closure_impl<Xs...> REF xs)                  \
            { return zip(static_cast<Xs REF>(xs).get...); }                 \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_UNZIP)
        #undef BOOST_HANA_PP_UNZIP


        // zip_with
        ///////////////
        #define BOOST_HANA_PP_ZIP_WITH1(REF)                                \
            template <typename F, typename ...Xs>                           \
            static constexpr decltype(auto) zip_with_helper(F&& f,          \
                detail::closure_impl<Xs...> REF xs)                         \
            {                                                               \
                return tuple(f(static_cast<Xs REF>(xs).get)...);            \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_ZIP_WITH1)
        #undef BOOST_HANA_PP_ZIP_WITH1

        #define BOOST_HANA_PP_ZIP_WITH2(REF1, REF2)                         \
            template <typename F, typename ...Xs, typename ...Ys>           \
            static constexpr decltype(auto) zip_with_helper(F&& f,          \
                detail::closure_impl<Xs...> REF1 xs,                        \
                detail::closure_impl<Ys...> REF2 ys)                        \
            {                                                               \
                return tuple(                                               \
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
                return tuple(                                                     \
                    f(static_cast<Xs REF1>(xs).get,                               \
                      static_cast<Ys REF2>(ys).get,                               \
                      static_cast<Zs REF3>(zs).get)...                            \
                );                                                                \
            }                                                                     \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF3(BOOST_HANA_PP_ZIP_WITH3)
        #undef BOOST_HANA_PP_ZIP_WITH3

        template <typename F, typename X1, typename X2, typename X3, typename X4, typename ...Xn>
        static constexpr decltype(auto) zip_with_helper(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xn&& ...xn) {
            return zip_with_helper(apply,
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
        static constexpr decltype(auto) zip_with_impl(F&& f, Xs&& xs, Ys&& ...ys) {
            constexpr detail::std::size_t lengths[] = {
                get_size(decltype(&xs){}), get_size(decltype(&ys){})...
            };
            constexpr detail::std::size_t zip_length = homogeneous_min(lengths);
            return zip_with_helper(detail::std::forward<F>(f),
                take_c<zip_length>(detail::std::forward<Xs>(xs)),
                take_c<zip_length>(detail::std::forward<Ys>(ys))...
            );
        }


        // zip
        ///////////////
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
            return zip_with_helper(concat,
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
        static constexpr decltype(auto) zip_impl(Xs&& xs, Ys&& ...ys) {
            constexpr detail::std::size_t lengths[] = {
                get_size(decltype(&xs){}), get_size(decltype(&ys){})...
            };
            constexpr detail::std::size_t zip_length = homogeneous_min(lengths);
            return zip_helper(
                take_c<zip_length>(detail::std::forward<Xs>(xs)),
                take_c<zip_length>(detail::std::forward<Ys>(ys))...
            );
        }
    };

    template <>
    struct fmap_impl<Tuple> {
        #define BOOST_HANA_PP_FMAP(REF)                                     \
            template <typename ...Xs, typename F>                           \
            static constexpr decltype(auto)                                 \
            apply(detail::closure_impl<Xs...> REF xs, F&& f) {              \
                return tuple(f(static_cast<Xs REF>(xs).get)...);            \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_FMAP)
        #undef BOOST_HANA_PP_FMAP
    };

    template <>
    struct fill_impl<Tuple> : tuple_detail::utils {
        template <typename V>
        static constexpr _tuple<> apply(_tuple<> const&, V&& v)
        { return {}; }

        template <typename X, typename ...Xs, typename V>
        static constexpr _tuple<
            typename detail::std::decay<V>::type,
            allow_expansion<(sizeof(Xs), false), typename detail::std::decay<V>::type>...
        > apply(_tuple<X, Xs...> const&, V&& v) {
            return {((void)sizeof(Xs), v)..., detail::std::forward<V>(v)};
        }
    };

#undef BOOST_HANA_PP_FOR_EACH_REF1
#undef BOOST_HANA_PP_FOR_EACH_REF2
#undef BOOST_HANA_PP_FOR_EACH_REF3

#if 0


    // As an optimization, we also provide the same operations but with the last
    // argument as a parameter pack instead of a closure. This sometimes allows us
    // to avoid creating a temporary closure just for passing it to the function.
    namespace from_variadic {
        template <typename Into, typename F, typename ...Xs>
        constexpr decltype(auto) zip_with1(Into&& into, F&& f, Xs const& ...xs) {
            return into(f(xs)...);
        }

        template <typename Into, typename F, typename ...Xs, typename ...Ys>
        constexpr decltype(auto) zip_with2(Into&& into, F&& f, closure<Xs...> const& xs, Ys const& ...ys) {
            return into(f(static_cast<Xs const&>(xs).get, ys)...);
        }

        template <typename Into, typename F, typename ...Xs, typename ...Ys, typename ...Zs>
        constexpr decltype(auto) zip_with3(Into&& into, F&& f, closure<Xs...> const& xs, closure<Ys...> const& ys, Zs const& ...zs) {
            return into(f(static_cast<Xs const&>(xs).get, static_cast<Ys const&>(ys).get, zs)...);
        }


        template <typename Into, typename ...Xs>
        constexpr decltype(auto) unpack(Into&& into, Xs const& ...xs) {
            return into(xs...);
        }


        template <typename Into, typename ...Xs, typename ...Ys>
        constexpr decltype(auto) concat2(Into&& into, closure<Xs...> const& xs, Ys... const& ys) {
            return into(static_cast<Xs const&>(xs).get..., ys...);
        }

        template <typename Into, typename ...Xs, typename ...Ys, typename ...Zs>
        constexpr decltype(auto) concat3(Into&& into, closure<Xs...> const& xs, closure<Ys...> const& ys, Zs const& ...zs) {
            return into(static_cast<Xs const&>(xs).get..., static_cast<Ys const&>(ys).get..., zs...);
        }


        template <std::size_t n, typename ...Xs>
        constexpr decltype(auto) at(Xs const& ...xs) {
            return variadic::at<n>(xs...);
        }
    }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_HPP
