/*!
@file
Defines `boost::hana::Tuple`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_HPP
#define BOOST_HANA_TUPLE_HPP

#include <boost/hana/fwd/tuple.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/detail/constexpr/algorithm.hpp>
#include <boost/hana/detail/constexpr/array.hpp>
#include <boost/hana/detail/generate_integer_sequence.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/iterable.hpp>
#include <boost/hana/detail/operators/monad.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/detail/type_foldl1.hpp>
#include <boost/hana/detail/type_foldr1.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/detail/variadic/foldr1.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // tuple, tuple_t and tuple_c
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Xs>
    struct _tuple
        : detail::closure<Xs...>
        , operators::adl
        , detail::iterable_operators<_tuple<Xs...>>
    {
        using detail::closure<Xs...>::closure; // inherit constructor
        _tuple() = default;
        _tuple(_tuple&&) = default;
        _tuple(_tuple const&) = default;
        _tuple(_tuple&) = default;

        using hana = _tuple;
        using datatype = Tuple;

        static constexpr std::size_t size = sizeof...(Xs);
        static constexpr bool is_tuple_t = false;
        static constexpr bool is_tuple_c = false;
    };

    template <typename T, T ...v>
    struct _tuple_c : _tuple<_integral_constant<T, v>...> {
        static constexpr bool is_tuple_c = true;
    };

    template <typename ...T>
    struct _tuple_t {
        struct _;
    };

    template <typename ...T>
    struct _tuple_t<T...>::_
        : _tuple_t<T...>,
          _tuple<typename std::remove_cv<decltype(gcc_wknd::mktype<T>())>::type...>
    {
        static constexpr bool is_tuple_t = true;
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<Tuple> {
            static constexpr bool value = true;
        };
        template <>
        struct orderable_operators<Tuple> {
            static constexpr bool value = true;
        };
        template <>
        struct monad_operators<Tuple> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // make
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Tuple> {
        template <typename ...Xs>
        static constexpr
        _tuple<typename std::decay<Xs>::type...> apply(Xs&& ...xs)
        { return {static_cast<Xs&&>(xs)...}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // utilities
    //////////////////////////////////////////////////////////////////////////
    namespace tuple_detail {
        template <typename Xs>
        using size = _integral_constant<std::size_t,
            std::remove_reference<Xs>::type::size
        >;

        template <bool, typename T>
        using expand = T;

        template <typename GenIndices, std::size_t ...i>
        constexpr auto
        generate_index_sequence_impl(std::index_sequence<i...>) {
            constexpr auto indices = GenIndices::apply();
            (void)indices; // remove GCC warning about `indices` being unused
            return std::index_sequence<indices[i]...>{};
        }

        template <typename GenIndices>
        constexpr auto generate_index_sequence() {
            constexpr auto indices = GenIndices::apply();
            return generate_index_sequence_impl<GenIndices>(
                std::make_index_sequence<indices.size()>{});
        }

        template <typename Xs, std::size_t ...i>
        constexpr auto get_subsequence(Xs&& xs, std::index_sequence<i...>) {
            return hana::make_tuple(detail::get<i>(static_cast<Xs&&>(xs))...);
        }
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

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Tuple, Tuple> {
        // tuple_t
        template <typename ...T>
        static constexpr auto apply(_tuple_t<T...> const&,
                                    _tuple_t<T...> const&)
        { return true_; }

        // tuple_c
        template <typename V, V ...v, typename U, U ...u, typename =
            std::enable_if_t<sizeof...(v) == sizeof...(u)>>
        static constexpr auto apply(_tuple_c<V, v...> const&,
                                    _tuple_c<U, u...> const&)
        {
            constexpr bool comparisons[] = {true, (v == u)...};
            return bool_<hana::all(comparisons)>;
        }

        // tuple
        template <std::size_t i, std::size_t n>
        struct equal_tuple {
            template <typename Xs, typename Ys>
            constexpr decltype(auto) operator()(Xs const* xs, Ys const* ys) const {
                return hana::eval_if(
                    hana::equal(detail::get<i>(*xs), detail::get<i>(*ys)),
                    hana::lazy(equal_tuple<i+1, n>{})(xs, ys),
                    hana::lazy(false_)
                );
            }
        };

        template <std::size_t n>
        struct equal_tuple<n, n> {
            constexpr auto operator()(...) const
            { return true_; }
        };

        template <typename Xs, typename Ys, typename = std::enable_if_t<
            (Xs::size == Ys::size && Xs::size != 0) &&
            !(Xs::is_tuple_t && Ys::is_tuple_t) &&
            !(Xs::is_tuple_c && Ys::is_tuple_c)
        >>
        static constexpr decltype(auto) apply(Xs const& xs, Ys const& ys)
        { return equal_tuple<0, Xs::size>{}(&xs, &ys); }


        // empty tuples and tuples with different sizes
        template <typename Xs, typename Ys, typename = std::enable_if_t<
            Xs::size != Ys::size || Xs::size == 0
        >>
        static constexpr decltype(auto) apply(Xs const& /*xs*/, Ys const& /*ys*/, ...)
        { return bool_<Xs::size == 0 && Ys::size == 0>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Tuple> {
        struct unpack_tuple_t_metafunction {
            template <typename ...T, template <typename ...> class F>
            constexpr auto operator()(_tuple_t<T...> const&, _metafunction<F> const&) const
            { return type<typename F<T...>::type>; }
        };

        struct unpack_tuple {
            #define BOOST_HANA_PP_UNPACK(REF)                                   \
                template <typename ...Xs, typename F>                           \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<Xs...> REF xs, F&& f) const     \
                { return static_cast<F&&>(f)(static_cast<Xs REF>(xs).get...); } \
            /**/
            BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_UNPACK)
            #undef BOOST_HANA_PP_UNPACK
        };

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return overload_linearly(
                unpack_tuple_t_metafunction{},
                unpack_tuple{}
            )(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    template <>
    struct fold_left_impl<Tuple> {
        struct fold_left_tuple {
            #define BOOST_HANA_PP_FOLD_LEFT(REF)                                \
                template <typename ...Xs, typename S, typename F>               \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<Xs...> REF xs, S&& s, F&& f) const {\
                    return detail::variadic::foldl1(                            \
                        static_cast<F&&>(f),                                    \
                        static_cast<S&&>(s),                                    \
                        static_cast<Xs REF>(xs).get...                          \
                    );                                                          \
                }                                                               \
                                                                                \
                template <typename ...Xs, typename F>                           \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<Xs...> REF xs, F&& f) const {   \
                    return detail::variadic::foldl1(                            \
                        static_cast<F&&>(f),                                    \
                        static_cast<Xs REF>(xs).get...                          \
                    );                                                          \
                }                                                               \
            /**/
            BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_FOLD_LEFT)
            #undef BOOST_HANA_PP_FOLD_LEFT
        };

        struct fold_left_tuple_t {
            template <typename ...T, typename S, template <typename ...> class F>
            constexpr auto
            operator()(_tuple_t<T...> const&, _type<S> const&, _metafunction<F> const&) const {
                return type<typename detail::type_foldl1<F, S, T...>::type>;
            }
            template <typename ...T, template <typename ...> class F>
            constexpr auto
            operator()(_tuple_t<T...> const&, _metafunction<F> const&) const {
                return type<typename detail::type_foldl1<F, T...>::type>;
            }
        };

        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return overload_linearly(
                fold_left_tuple_t{},
                fold_left_tuple{}
            )(static_cast<Xs&&>(xs), static_cast<S&&>(s), static_cast<F&&>(f));
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return overload_linearly(
                fold_left_tuple_t{},
                fold_left_tuple{}
            )(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    template <>
    struct fold_right_impl<Tuple> {
        struct fold_right_tuple {
            #define BOOST_HANA_PP_FOLD_RIGHT(REF)                               \
                template <typename ...Xs, typename S, typename F>               \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<Xs...> REF xs, S&& s, F&& f) const {\
                    return detail::variadic::foldr1(                            \
                        static_cast<F&&>(f),                                    \
                        static_cast<Xs REF>(xs).get...,                         \
                        static_cast<S&&>(s)                                     \
                    );                                                          \
                }                                                               \
                                                                                \
                template <typename ...Xs, typename F>                           \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<Xs...> REF xs, F&& f) const {   \
                    return detail::variadic::foldr1(                            \
                        static_cast<F&&>(f),                                    \
                        static_cast<Xs REF>(xs).get...                          \
                    );                                                          \
                }                                                               \
            /**/
            BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_FOLD_RIGHT)
            #undef BOOST_HANA_PP_FOLD_RIGHT
        };

        struct fold_right_tuple_t {
            template <typename ...T, typename S, template <typename ...> class F>
            constexpr auto
            operator()(_tuple_t<T...> const&, _type<S> const&, _metafunction<F> const&) const {
                return type<typename detail::type_foldr1<F, T..., S>::type>;
            }

            template <typename ...T, template <typename ...> class F>
            constexpr auto
            operator()(_tuple_t<T...> const&, _metafunction<F> const&) const {
                return type<typename detail::type_foldr1<F, T...>::type>;
            }
        };

        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return overload_linearly(
                fold_right_tuple_t{},
                fold_right_tuple{}
            )(static_cast<Xs&&>(xs), static_cast<S&&>(s), static_cast<F&&>(f));
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return overload_linearly(
                fold_right_tuple_t{},
                fold_right_tuple{}
            )(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    template <>
    struct length_impl<Tuple> {
        template <typename Xs>
        static constexpr _size_t<Xs::size> apply(Xs const&)
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct head_impl<Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return detail::get<0>(static_cast<Xs&&>(xs)); }
    };

    template <>
    struct at_impl<Tuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t index = hana::value<N>();
            return detail::get<index>(static_cast<Xs&&>(xs));
        }
    };

    template <>
    struct tail_impl<Tuple> {
        struct tail_tuple {
            #define BOOST_HANA_PP_TAIL(REF)                                 \
                template <typename X, typename ...Xn>                       \
                constexpr _tuple<typename Xn::get_type...>                  \
                operator()(detail::closure_impl<X, Xn...> REF xs) const {   \
                    return {static_cast<Xn REF>(xs).get...};                \
                }                                                           \
            /**/
            BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_TAIL)
            #undef BOOST_HANA_PP_TAIL
        };

        struct tail_tuple_t {
            template <typename T, typename ...Ts>
            constexpr auto operator()(_tuple_t<T, Ts...>) const
            { return tuple_t<Ts...>; }
        };

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return overload_linearly(
                tail_tuple_t{},
                tail_tuple{}
            )(static_cast<Xs&&>(xs));
        }
    };

    template <>
    struct is_empty_impl<Tuple> {
        template <typename Xs>
        static constexpr auto apply(Xs const&)
        { return bool_<Xs::size == 0>; }
    };

    template <>
    struct last_impl<Tuple> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            constexpr std::size_t size = tuple_detail::size<Xs>{}();
            return detail::get<size - 1>(static_cast<Xs&&>(xs));
        }
    };

    template <>
    struct drop_at_most_impl<Tuple> {
        using Size = std::size_t;

        template <Size n, typename Xs, Size ...i>
        static constexpr decltype(auto)
        drop_helper(Xs&& xs, std::index_sequence<i...>) {
            return hana::make<Tuple>(detail::get<n + i>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr Size n = hana::value<N>();
            constexpr Size size = tuple_detail::size<Xs>{}();
            constexpr Size drop_size = n > size ? size : n;
            return drop_helper<drop_size>(static_cast<Xs&&>(xs),
                std::make_index_sequence<size - drop_size>{});
        }
    };

    template <>
    struct drop_while_impl<Tuple> {
        using Size = std::size_t;

        template <Size k, Size Len>
        struct drop_while_helper {
            template <typename Xs, typename Pred>
            static constexpr decltype(auto)
            apply(decltype(false_), Xs&& xs, Pred&&) {
                return hana::drop.exactly(static_cast<Xs&&>(xs),
                                          hana::size_t<k-1>);
            }

            template <typename Xs, typename Pred>
            static constexpr decltype(auto)
            apply(decltype(true_), Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::at_c<k>(xs)), true_, false_);
                return drop_while_helper<k + 1, Len>::apply(cond,
                                        static_cast<Xs&&>(xs),
                                        static_cast<Pred&&>(pred));
            }
        };

        template <Size Len>
        struct drop_while_helper<Len, Len> {
            template <typename Xs, typename Pred>
            static constexpr decltype(auto)
            apply(decltype(false_), Xs&& xs, Pred&&) {
                return hana::drop.exactly(static_cast<Xs&&>(xs),
                                          hana::size_t<Len - 1>);
            }

            template <typename Xs, typename Pred>
            static constexpr decltype(auto)
            apply(decltype(true_), Xs&& xs, Pred&&) {
                return hana::drop.exactly(static_cast<Xs&&>(xs),
                                          hana::size_t<Len>);
            }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr Size len = tuple_detail::size<Xs>::value;
            return drop_while_helper<0, len>::apply(true_,
                                            static_cast<Xs&&>(xs),
                                            static_cast<Pred&&>(pred));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    namespace tuple_detail {
        template <typename Xs, typename Pred, typename = hana::when<true>>
        struct find_tail_size;

        template <typename Pred>
        struct find_tail_size<hana::_tuple<>, Pred> { };

        template <typename X, typename ...Xs, typename Pred>
        struct find_tail_size<hana::_tuple<X, Xs...>, Pred, hana::when<
            hana::if_(hana::value<decltype(
                std::declval<Pred>()(std::declval<X>())
            )>(), true, false)
        >> {
            static constexpr auto value = sizeof...(Xs);
        };

        template <typename X, typename ...Xs, typename Pred>
        struct find_tail_size<hana::_tuple<X, Xs...>, Pred, hana::when<
            !hana::if_(hana::value<decltype(
                std::declval<Pred>()(std::declval<X>())
            )>(), true, false)
        >>
            : find_tail_size<hana::_tuple<Xs...>, Pred>
        { };

        template <typename Xs, typename Pred,
            std::size_t tail_size = find_tail_size<
                typename std::remove_cv<
                    typename std::remove_reference<Xs>::type
                >::type, Pred
            >::value
        >
        constexpr auto find_if_impl(Xs&& xs, Pred const&, int) {
            constexpr auto size = tuple_detail::size<Xs>::value;
            constexpr auto index = size - tail_size - 1;
            return hana::just(hana::at_c<index>(static_cast<Xs&&>(xs)));
        }

        template <typename Xs, typename Pred>
        constexpr auto find_if_impl(Xs&&, Pred const&, long)
        { return hana::nothing; }
    }

    template <>
    struct find_if_impl<Tuple> {
        #define BOOST_HANA_PP_FIND_IF(REF)                                  \
            template <typename ...Xs, typename Pred>                        \
            static constexpr decltype(auto)                                 \
            apply(_tuple<Xs...> REF xs, Pred&& pred) {                      \
                return tuple_detail::find_if_impl(                          \
                    static_cast<_tuple<Xs...> REF>(xs),                     \
                    static_cast<Pred&&>(pred), int{});                      \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_FIND_IF)
        #undef BOOST_HANA_PP_FIND_IF
    };

    template <>
    struct any_of_impl<Tuple> {
        template <std::size_t k, std::size_t Len>
        struct any_of_helper {
            template <typename Xs, typename Pred>
            static constexpr auto apply(bool prev_cond, Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(at_c<k>(xs)), true_, false_);
                return prev_cond ? true_
                    : any_of_impl::any_of_helper<k + 1, Len>::apply(cond,
                                        static_cast<Xs&&>(xs),
                                        static_cast<Pred&&>(pred));
            }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(true_), Xs&&, Pred&&)
            { return true_; }

            template <typename Xs, typename Pred>
            static constexpr auto apply(decltype(false_), Xs&& xs, Pred&& pred) {
                auto cond = hana::if_(pred(hana::at_c<k>(xs)), true_, false_);
                return any_of_impl::any_of_helper<k + 1, Len>::apply(cond,
                                        static_cast<Xs&&>(xs),
                                        static_cast<Pred&&>(pred));
            }
        };

        template <std::size_t Len>
        struct any_of_helper<Len, Len> {
            template <typename Cond, typename Xs, typename Pred>
            static constexpr auto apply(Cond cond, Xs&&, Pred&&)
            { return cond; }
        };

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            constexpr std::size_t len = hana::value<
                decltype(hana::length(xs))
            >();
            return any_of_impl::any_of_helper<0, len>::apply(false_,
                                            static_cast<Xs&&>(xs),
                                            static_cast<Pred&&>(pred));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<Tuple> {
        struct transform_tuple {
            #define BOOST_HANA_PP_TRANSFORM(REF)                                \
                template <typename ...Xs, typename F>                           \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<Xs...> REF xs, F&& f) const     \
                { return hana::make<Tuple>(f(static_cast<Xs REF>(xs).get)...); }\
                                                                                \
                template <typename X, typename F>                               \
                constexpr decltype(auto)                                        \
                operator()(detail::closure_impl<X> REF xs, F&& f) const {       \
                    return hana::make<Tuple>(static_cast<F&&>(f)(               \
                        static_cast<X REF>(xs).get                              \
                    ));                                                         \
                }                                                               \
            /**/
            BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_TRANSFORM)
            #undef BOOST_HANA_PP_TRANSFORM
        };

        struct transform_tuple_t_metafunction {
            template <typename ...T, template <typename ...> class F>
            constexpr auto operator()(_tuple_t<T...> const&, _metafunction<F> const&) const
            { return tuple_t<typename F<T>::type...>; }
        };

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return overload_linearly(
                transform_tuple_t_metafunction{},
                transform_tuple{}
            )(static_cast<Xs&&>(xs), static_cast<F&&>(f));
        }
    };

    template <>
    struct fill_impl<Tuple> {
        template <typename ...T>
        struct pack { };

        template <typename V>
        static constexpr _tuple<> helper(pack<> const&, V&&)
        { return {}; }

        template <typename X, typename ...Xs, typename V>
        static constexpr _tuple<
            typename std::decay<V>::type,
            typename std::decay<
                tuple_detail::expand<!!sizeof(Xs), V>
            >::type...
        > helper(pack<X, Xs...> const&, V&& v)
        { return {((void)sizeof(Xs), v)..., static_cast<V&&>(v)}; }

        template <typename ...Xs, typename T>
        static constexpr auto helper(pack<Xs...> const&, _type<T> const&)
        { return tuple_t<tuple_detail::expand<!!sizeof(Xs), T>...>; }

        template <typename ...T, template <typename ...> class tuple, typename V>
        static constexpr decltype(auto) apply(tuple<T...> const&, V&& v) {
            return helper(pack<T...>{}, static_cast<V&&>(v));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<Tuple> {
        // _tuple
        template <typename X>
        static constexpr _tuple<typename std::decay<X>::type> apply(X&& x)
        { return {static_cast<X&&>(x)}; }

        // tuple_t
        template <typename T>
        static constexpr auto apply(_type<T> const&)
        { return tuple_t<T>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<Tuple> {
        using Size = std::size_t;

        template <bool inner, Size ...Lengths>
        struct flatten_indices {
            static constexpr auto apply() {
                constexpr Size lengths[] = {Lengths...};
                constexpr Size total_length = hana::sum<Size>(lengths);
                detail::constexpr_::array<Size, total_length> indices{};

                for (Size index = 0, i = 0; i < sizeof...(Lengths); ++i) {
                    for (Size j = 0; j < lengths[i]; ++j, ++index) {
                        indices[index] = (inner ? i : j);
                    }
                }
                return indices;
            }
        };

        template <typename Xs, Size ...outer, Size ...inner>
        static constexpr decltype(auto)
        flatten_helper(Xs&& xs, std::index_sequence<outer...>,
                                std::index_sequence<inner...>)
        {
            return hana::make<Tuple>(detail::get<outer>(detail::get<inner>(
                                        static_cast<Xs&&>(xs)))...);
        }

        #define BOOST_HANA_PP_FLATTEN(REF)                                      \
            template <typename ...Xs>                                           \
            static constexpr decltype(auto)                                     \
            apply(detail::closure_impl<Xs...> REF xs) {                         \
                using tuple_detail::generate_index_sequence;                    \
                using tuple_detail::size;                                       \
                                                                                \
                auto outer = generate_index_sequence<                           \
                    flatten_indices<false, size<typename Xs::get_type>{}()...>  \
                >();                                                            \
                                                                                \
                auto inner = generate_index_sequence<                           \
                    flatten_indices<true, size<typename Xs::get_type>{}()...>   \
                >();                                                            \
                                                                                \
                return flatten_helper(                                          \
                    static_cast<detail::closure_impl<Xs...> REF>(xs),           \
                    outer, inner                                                \
                );                                                              \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_FLATTEN)
        #undef BOOST_HANA_PP_FLATTEN

        static constexpr _tuple<> apply(_tuple<> const&) { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct concat_impl<Tuple> {
        #define BOOST_HANA_PP_CONCAT_METHOD(REF1, REF2)                     \
            template <typename ...Xs, typename ...Ys>                       \
            static constexpr _tuple<                                        \
                typename Xs::get_type..., typename Ys::get_type...          \
            > apply(detail::closure_impl<Xs...> REF1 xs,                    \
                    detail::closure_impl<Ys...> REF2 ys)                    \
            {                                                               \
                return {static_cast<Xs REF1>(xs).get...,                    \
                        static_cast<Ys REF2>(ys).get...};                   \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF2(BOOST_HANA_PP_CONCAT_METHOD)
        #undef BOOST_HANA_PP_CONCAT_METHOD
    };

    template <>
    struct empty_impl<Tuple> {
        static constexpr _tuple<> apply()
        { return {}; }
    };

    template <>
    struct repeat_impl<Tuple> {
        template <typename X>
        static constexpr _tuple<>
        repeat_helper(X&&, std::index_sequence<>)
        { return {}; }

        template <typename X, std::size_t i, std::size_t ...j>
        static constexpr _tuple<
            typename std::decay<X>::type,
            tuple_detail::expand<!!j, typename std::decay<X>::type>...
        > repeat_helper(X&& x, std::index_sequence<i, j...>)
        { return {((void)j, x)..., static_cast<X&&>(x)}; }

        template <typename T, std::size_t ...i>
        static constexpr auto
        repeat_helper(_type<T>, std::index_sequence<i...>)
        { return make_tuple_t<tuple_detail::expand<!!i, T>...>(); }

        template <typename X, typename N>
        static constexpr decltype(auto) apply(X&& x, N const&) {
            constexpr std::size_t n = hana::value<N>();
            return repeat_helper(static_cast<X&&>(x),
                                 std::make_index_sequence<n>{});
        }
    };

    template <>
    struct cycle_impl<Tuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N&& n) {
            return hana::flatten(hana::repeat<Tuple>(
                static_cast<Xs&&>(xs),
                static_cast<N&&>(n)
            ));
        }
    };

    template <>
    struct prepend_impl<Tuple> {
        #define BOOST_HANA_PP_PREPEND(REF)                                      \
            template <typename ...Xs, typename X>                               \
            static constexpr _tuple<                                            \
                typename std::decay<X>::type, typename Xs::get_type...  \
            > apply(detail::closure_impl<Xs...> REF xs, X&& x) {                \
                return {                                                        \
                    static_cast<X&&>(x), static_cast<Xs REF>(xs).get...  \
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
                typename Xs::get_type..., typename std::decay<X>::type  \
            > apply(detail::closure_impl<Xs...> REF xs, X&& x) {                \
                return {                                                        \
                    static_cast<Xs REF>(xs).get..., static_cast<X&&>(x)  \
                };                                                              \
            }                                                                   \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_APPEND)
        #undef BOOST_HANA_PP_APPEND
    };

    template <>
    struct filter_impl<Tuple> {
        template <typename Pred, typename ...Xs>
        struct filter_helper {
            static constexpr auto apply() {
                using detail::constexpr_::count;
                using detail::constexpr_::array;
                using std::size_t;
                using std::declval;

                constexpr size_t N = sizeof...(Xs);
                constexpr array<bool, N> results ={{
                    hana::value<decltype(declval<Pred>()(declval<Xs>()))>()...
                }};
                constexpr size_t keptN = count(&results[0], &results[N], true);
                array<size_t, keptN> kept_indices{};
                size_t* keep = &kept_indices[0];
                for (size_t index = 0; index < N; ++index)
                    if (results[index])
                        *keep++ = index;
                return kept_indices;
            }
        };

        template <typename ...Xs, typename Pred>
        static constexpr auto
        filter_indices(detail::closure_impl<Xs...> const&, Pred&&) {
            using tuple_detail::generate_index_sequence;
            return generate_index_sequence<
                filter_helper<Pred&&, typename Xs::get_type...>
            >();
        }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            using tuple_detail::get_subsequence;
            auto indices = filter_impl::filter_indices(xs, static_cast<Pred&&>(pred));
            return get_subsequence(static_cast<Xs&&>(xs), indices);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, Tuple>
        : decltype(true_)
    { };

    // Credits: implementation adapted from http://github.com/alexk7/hel.
    template <>
    struct cartesian_product_impl<Tuple> {
        using Size = std::size_t;

        template <Size ...lengths_>
        static constexpr auto indices_of(Size i) {
            constexpr Size lengths[] = {lengths_...};
            constexpr Size n = sizeof...(lengths_);
            detail::constexpr_::array<Size, n> result{};
            for (Size j = n; j--;) {
                result[j] = i % lengths[j];
                i /= lengths[j];
            }
            return result;
        }

        template <Size n, Size ...k, typename ...Tuples>
        static constexpr auto
        product_element(std::index_sequence<k...>, Tuples&& ...tuples) {
            constexpr auto indices = indices_of<tuple_detail::size<Tuples>{}()...>(n);
            return hana::make_tuple(detail::get<indices[k]>(tuples)...);
        }

        template <Size ...n, typename ...Tuples>
        static constexpr auto
        cartesian_product_helper(std::index_sequence<n...>,
                                 Tuples&& ...tuples)
        {
            auto ks = std::make_index_sequence<sizeof...(Tuples)>{};
            return hana::make_tuple(product_element<n>(ks, tuples...)...);
        }

        #define BOOST_HANA_PP_CARTESIAN_PRODUCT(REF)                                \
            template <typename ...Tuples>                                           \
            static constexpr auto apply(detail::closure_impl<Tuples...> REF xs) {   \
                constexpr Size lengths[] = {                                        \
                    tuple_detail::size<typename Tuples::get_type>{}()...            \
                };                                                                  \
                constexpr Size total_length = hana::product<Size>(lengths);         \
                auto ns = std::make_index_sequence<total_length>{};         \
                return cartesian_product_helper(ns,                                 \
                    static_cast<Tuples REF>(xs).get...                              \
                );                                                                  \
            }                                                                       \
                                                                                    \
            static constexpr auto apply(detail::closure_impl<> REF) {               \
                return hana::make_tuple();                                          \
            }                                                                       \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_CARTESIAN_PRODUCT)
        #undef BOOST_HANA_PP_CARTESIAN_PRODUCT
    };

    template <>
    struct init_impl<Tuple> {
        template <typename Xs, std::size_t ...n>
        static constexpr decltype(auto)
        init_helper(Xs&& xs, std::index_sequence<n...>) {
            return hana::make<Tuple>(
                        detail::get<n>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            constexpr std::size_t size = tuple_detail::size<Xs>{}();
            return init_helper(static_cast<Xs&&>(xs),
                std::make_index_sequence<size - 1>{});
        }
    };

    template <>
    struct intersperse_impl<Tuple> {
        template <std::size_t i, typename Z, typename Xs>
        static constexpr Z pick(Z&& z, Xs const&,
            decltype(false_) /* odd index */)
        { return static_cast<Z&&>(z); }

        template <std::size_t i, typename Z, typename Xn>
        static constexpr Xn const&
        pick(Z const&, detail::element<(i + 1) / 2, Xn> const& x,
            decltype(true_) /* even index */)
        { return x.get; }

        template <std::size_t i, typename Z, typename Xn>
        static constexpr Xn&&
        pick(Z const&, detail::element<(i + 1) / 2, Xn>&& x,
            decltype(true_) /* even index */)
        { return static_cast<detail::element<(i + 1) / 2, Xn>&&>(x).get; }

        template <typename Xs, typename Z, std::size_t ...i>
        static constexpr decltype(auto)
        intersperse_helper(Xs&& xs, Z&& z, std::index_sequence<i...>) {
            return hana::make<Tuple>(
                pick<i>(
                    static_cast<Z&&>(z),
                    static_cast<Xs&&>(xs),
                    bool_<(i % 2 == 0)>
                )...
            );
        }

        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            constexpr std::size_t size = tuple_detail::size<Xs>{}();
            constexpr auto new_seq_size = size == 0 ? 0 : (size * 2) - 1;
            return intersperse_helper(static_cast<Xs&&>(xs),
                          static_cast<Z&&>(z),
                          std::make_index_sequence<new_seq_size>{});
        }
    };

    namespace tuple_detail {
        template <std::size_t N>
        constexpr auto permutation_indices =
            detail::constexpr_::array<std::size_t, N>{}.iota(0)
                                                               .permutations();
    }

    template <>
    struct permutations_impl<Tuple> {
        using Size = std::size_t;

        template <Size n, typename Xs, Size ...i>
        static constexpr auto
        nth_permutation(Xs const& xs, std::index_sequence<i...>) {
            return hana::make<Tuple>(
                detail::get<tuple_detail::permutation_indices<Xs::size>[n][i]>(xs)...
            );
        }

        template <typename Xs, Size ...n>
        static constexpr auto
        permutations_helper(Xs const& xs, std::index_sequence<n...>) {
            return hana::make<Tuple>(
                nth_permutation<n>(
                    xs, std::make_index_sequence<Xs::size>{}
                )...
            );
        }

        template <typename Xs>
        static constexpr auto apply(Xs const& xs) {
            constexpr Size total_permutations =
                detail::constexpr_::factorial(Xs::size);
            return permutations_helper(xs,
                std::make_index_sequence<total_permutations>{}
            );
        }
    };

    template <>
    struct remove_at_impl<Tuple> {
        using Size = std::size_t;

        template <typename Xs, Size ...before, Size ...after>
        static constexpr decltype(auto)
        remove_at_helper(Xs&& xs, std::index_sequence<before...>,
                                  std::index_sequence<after...>)
        {
            return hana::make<Tuple>(
                detail::get<before>(
                    static_cast<Xs&&>(xs)
                )...,
                detail::get<sizeof...(before) + after + 1>(
                    static_cast<Xs&&>(xs)
                )...
            );
        }

        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr Size index = hana::value<N>();
            constexpr Size size = tuple_detail::size<Xs>{}();
            return remove_at_helper(static_cast<Xs&&>(xs),
                          std::make_index_sequence<index>{},
                          std::make_index_sequence<size - index - 1>{});
        }
    };

    template <>
    struct partition_impl<Tuple> {
        template <int which, typename Pred, typename ...Xs>
        struct partition_helper {
            static constexpr auto apply() {
                using detail::constexpr_::count;
                using detail::constexpr_::array;
                using std::size_t;
                using std::declval;

                constexpr size_t N = sizeof...(Xs);
                constexpr array<bool, N> results = {{
                    hana::value<decltype(declval<Pred>()(declval<Xs>()))>()...
                }};
                constexpr size_t leftN = count(&results[0], &results[N], true);
                array<size_t, leftN> left_indices{};
                array<size_t, N - leftN> right_indices{};
                size_t* left = &left_indices[0];
                size_t* right = &right_indices[0];
                for (size_t index = 0; index < N; ++index)
                    *(results[index] ? left : right)++ = index;
                return detail::get<which>(hana::make_tuple(left_indices,
                                                           right_indices));
            }
        };

        template <int which, typename ...Xs, typename Pred>
        static constexpr auto
        partition_indices(detail::closure_impl<Xs...> const&, Pred&&) {
            using tuple_detail::generate_index_sequence;
            return generate_index_sequence<
                partition_helper<which, Pred&&, typename Xs::get_type...>
            >();
        }

        template <typename Xs, typename Pred>
        static constexpr auto apply(Xs&& xs, Pred&& pred) {
            using tuple_detail::get_subsequence;
            auto left = partition_indices<0>(xs, static_cast<Pred&&>(pred));
            auto right = partition_indices<1>(xs, static_cast<Pred&&>(pred));
            return hana::make_pair(
                get_subsequence(static_cast<Xs&&>(xs), left),
                get_subsequence(static_cast<Xs&&>(xs), right)
            );
        }
    };

    template <>
    struct reverse_impl<Tuple> {
        template <std::size_t ...n, typename ...Xn>
        static constexpr decltype(auto)
        apply(detail::closure_impl<detail::element<n, Xn>...>&& xs) {
            using Closure = detail::closure_impl<detail::element<n, Xn>...>;
            return hana::make<Tuple>(detail::get<sizeof...(n) - n - 1>(
                                            static_cast<Closure&&>(xs))...);
        }

        template <std::size_t ...n, typename ...Xn>
        static constexpr decltype(auto)
        apply(detail::closure_impl<detail::element<n, Xn>...> const& xs) {
            return hana::make<Tuple>(detail::get<sizeof...(n) - n - 1>(xs)...);
        }
    };

    template <>
    struct slice_impl<Tuple> {
        template <std::size_t from, typename Xs, std::size_t ...i>
        static constexpr decltype(auto)
        slice_helper(Xs&& xs, std::index_sequence<i...>) {
            return hana::make<Tuple>(detail::get<from + i>(
                                        static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename From, typename To>
        static constexpr decltype(auto) apply(Xs&& xs, From const&, To const&) {
            constexpr std::size_t from = hana::value<From>();
            constexpr std::size_t to = hana::value<To>();
            return slice_helper<from>(static_cast<Xs&&>(xs),
                                std::make_index_sequence<to - from>{});
        }
    };

    template <>
    struct sort_impl<Tuple> {
        template <typename T, T ...v, std::size_t ...i>
        static constexpr auto
        sort_helper(_tuple_c<T, v...> const& /*xs*/,
                    std::index_sequence<i...>)
        {
            constexpr detail::constexpr_::array<T, sizeof...(v)> a{{v...}};
            constexpr auto result = a.sort();
            return tuple_c<T, result[i]...>;
        }

        template <typename ...T>
        static constexpr decltype(auto) apply(_tuple<T...> const& xs)
        { return hana::sort(xs, less); }

        template <typename ...T>
        static constexpr decltype(auto) apply(_tuple<T...>&& xs)
        { return hana::sort(std::move(xs), less); }

        template <typename T, T ...v>
        static constexpr auto apply(_tuple_c<T, v...> const& xs) {
            return sort_helper(xs,
                    std::make_index_sequence<sizeof...(v)>{});
        }
    };

    template <>
    struct take_impl<Tuple> {
        template <typename Xs, std::size_t ...n>
        static constexpr decltype(auto)
        take_helper(Xs&& xs, std::index_sequence<n...>) {
            return hana::make<Tuple>(detail::get<n>(static_cast<Xs&&>(xs))...);
        }

        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const&) {
            constexpr std::size_t n = hana::value<N>();
            constexpr std::size_t size = tuple_detail::size<Xs>{}();
            return take_helper(static_cast<Xs&&>(xs),
                std::make_index_sequence<(n < size ? n : size)>{});
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
    struct zip_unsafe_with_impl<Tuple> {
        #define BOOST_HANA_PP_ZIP_WITH1(REF)                                \
            template <typename F, typename ...Xs>                           \
            static constexpr decltype(auto) apply(F&& f,                    \
                detail::closure_impl<Xs...> REF xs)                         \
            { return hana::make<Tuple>(f(static_cast<Xs REF>(xs).get)...); }\
        /**/
        BOOST_HANA_PP_FOR_EACH_REF1(BOOST_HANA_PP_ZIP_WITH1)
        #undef BOOST_HANA_PP_ZIP_WITH1

        #define BOOST_HANA_PP_ZIP_WITH2(REF1, REF2)                         \
            template <typename F, typename ...Xs, typename ...Ys>           \
            static constexpr decltype(auto) apply(F&& f,                    \
                detail::closure_impl<Xs...> REF1 xs,                        \
                detail::closure_impl<Ys...> REF2 ys)                        \
            {                                                               \
                return hana::make<Tuple>(                                   \
                    f(static_cast<Xs REF1>(xs).get,                         \
                      static_cast<Ys REF2>(ys).get)...                      \
                );                                                          \
            }                                                               \
        /**/
        BOOST_HANA_PP_FOR_EACH_REF2(BOOST_HANA_PP_ZIP_WITH2)
        #undef BOOST_HANA_PP_ZIP_WITH2

        #define BOOST_HANA_PP_ZIP_WITH3(REF1, REF2, REF3)                         \
            template <typename F, typename ...Xs, typename ...Ys, typename ...Zs> \
            static constexpr decltype(auto) apply(F&& f,                          \
                detail::closure_impl<Xs...> REF1 xs,                              \
                detail::closure_impl<Ys...> REF2 ys,                              \
                detail::closure_impl<Zs...> REF3 zs)                              \
            {                                                                     \
                return hana::make<Tuple>(                                         \
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
        apply(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xn&& ...xn) {
            return zip_unsafe_with_impl::apply(hana::apply,
                zip_unsafe_with_impl::apply(
                    curry<sizeof...(xn) + 4>(static_cast<F&&>(f)),
                    static_cast<X1&&>(x1),
                    static_cast<X2&&>(x2),
                    static_cast<X3&&>(x3)
                ),
                static_cast<X4&&>(x4),
                static_cast<Xn&&>(xn)...
            );
        }
    };

    template <>
    struct zip_unsafe_impl<Tuple> {
        #define BOOST_HANA_PP_ZIP1(REF)                                     \
            template <typename ...Xs>                                       \
            static constexpr _tuple<                                        \
                _tuple<typename Xs::get_type>...                            \
            > apply(detail::closure_impl<Xs...> REF xs) {                   \
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
            > apply(detail::closure_impl<Xs...> REF1 xs,                    \
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
            > apply(detail::closure_impl<Xs...> REF1 xs,                \
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
        apply(X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xn&& ...xn) {
            return hana::zip.with(concat,
                zip_unsafe_impl::apply(
                    static_cast<X1&&>(x1),
                    static_cast<X2&&>(x2),
                    static_cast<X3&&>(x3)
                ),
                zip_unsafe_impl::apply(
                    static_cast<X4&&>(x4),
                    static_cast<Xn&&>(xn)...
                )
            );
        }
    };

#undef BOOST_HANA_PP_FOR_EACH_REF1
#undef BOOST_HANA_PP_FOR_EACH_REF2
#undef BOOST_HANA_PP_FOR_EACH_REF3
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_HPP
