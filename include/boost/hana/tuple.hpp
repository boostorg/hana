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
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/constant.hpp>
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
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/overload_linearly.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/concept/sequence.hpp>
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
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models_impl<Sequence, Tuple>
        : decltype(true_)
    { };

#undef BOOST_HANA_PP_FOR_EACH_REF1
#undef BOOST_HANA_PP_FOR_EACH_REF2
#undef BOOST_HANA_PP_FOR_EACH_REF3
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_HPP
