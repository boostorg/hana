/*!
@file
Defines the `Logical` and `Comparable` models of `boost::hana::integral_constant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_HPP
#define BOOST_HANA_BOOL_HPP

#include <boost/hana/fwd/bool.hpp>

#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/operators/arithmetic.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/logical.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/eval.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/eval_if.hpp>
#include <boost/hana/fwd/if.hpp>
#include <boost/hana/fwd/value.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // integral_constant
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    namespace ic_detail {
        template <typename T, T N, typename = std::make_integer_sequence<T, N>>
        struct go;

        template <typename T, T N, T ...i>
        struct go<T, N, std::integer_sequence<T, i...>> {
            using swallow = T[];

            template <typename F>
            static constexpr void with_index(F&& f)
            { (void)swallow{T{}, ((void)f(integral_constant<T, i>{}), i)...}; }

            template <typename F>
            static constexpr void without_index(F&& f)
            { (void)swallow{T{}, ((void)f(), i)...}; }
        };

        template <typename T, T v>
        template <typename F>
        constexpr void with_index_t<T, v>::operator()(F&& f) const
        { go<T, ((void)sizeof(&f), v)>::with_index(static_cast<F&&>(f)); }

        template <typename T, T v>
        template <typename F>
        constexpr void times_t<T, v>::operator()(F&& f) const
        { go<T, ((void)sizeof(&f), v)>::without_index(static_cast<F&&>(f)); }

        // avoid link-time error
        template <typename T, T v>
        constexpr with_index_t<T, v> times_t<T, v>::with_index;
    }

    // avoid link-time error
    template <typename T, T v>
    constexpr ic_detail::times_t<T, v> integral_constant<T, v>::times;

    template <typename T, T v>
    struct datatype<integral_constant<T, v>> {
        using type = IntegralConstant<T>;
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename T>
        struct comparable_operators<IntegralConstant<T>> {
            static constexpr bool value = true;
        };
        template <typename T>
        struct orderable_operators<IntegralConstant<T>> {
            static constexpr bool value = true;
        };
        template <typename T>
        struct arithmetic_operators<IntegralConstant<T>> {
            static constexpr bool value = true;
        };
        template <typename T>
        struct logical_operators<IntegralConstant<T>> {
            static constexpr bool value = true;
        };
    }

#define BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(op)                          \
    template <typename U, U u, typename V, V v>                             \
    constexpr integral_constant<decltype(u op v), (u op v)>                 \
    operator op(integral_constant<U, u>, integral_constant<V, v>)           \
    { return {}; }                                                          \
    /**/

#define BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(op)                           \
    template <typename U, U u>                                              \
    constexpr integral_constant<decltype(op u), (op u)>                     \
    operator op(integral_constant<U, u>)                                    \
    { return {}; }                                                          \
    /**/

    // Arithmetic
    BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(+)

    // Bitwise
    BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(~)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(&)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(|)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(^)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(<<)
    BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(>>)

#undef BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP
#undef BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP


    //////////////////////////////////////////////////////////////////////////
    // User-defined literal
    //////////////////////////////////////////////////////////////////////////
    namespace ic_detail {
        constexpr int to_int(char c)
        { return static_cast<int>(c) - 48; }

        template <std::size_t N>
        constexpr long long parse(const char (&arr)[N]) {
            long long number = 0, base = 1;
            for (std::size_t i = 0; i < N; ++i) {
                number += to_int(arr[N - 1 - i]) * base;
                base *= 10;
            }
            return number;
        }
    }

    namespace literals {
        template <char ...c>
        constexpr auto operator"" _c()
        { return llong_c<ic_detail::parse<sizeof...(c)>({c...})>; }
    }

    //////////////////////////////////////////////////////////////////////////
    // Model of Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<IntegralConstant<T>> {
        template <typename C>
        static constexpr auto apply()
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<IntegralConstant<T>, C, when<
        _models<Constant, C>::value &&
        std::is_integral<typename C::value_type>::value
    >>
        : embedding<is_embedded<typename C::value_type, T>::value>
    {
        static_assert(std::is_integral<T>::value,
        "trying to convert a Constant to an integral_constant of a non-integral "
        "type; hana::integral_constant may only hold integral types");

        template <typename X>
        static constexpr auto apply(X const&) {
            constexpr T v = hana::value<X>();
            return integral_constant<T, v>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Optimizations
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct eval_if_impl<IntegralConstant<T>> {
        template <typename Cond, typename Then, typename Else>
        static constexpr decltype(auto)
        apply(Cond const&, Then&& t, Else&& e) {
            return eval_if_impl::apply(hana::bool_c<static_cast<bool>(Cond::value)>,
                    static_cast<Then&&>(t), static_cast<Else&&>(e));
        }

        template <typename Then, typename Else>
        static constexpr decltype(auto)
        apply(decltype(hana::true_c) const&, Then&& t, Else&&)
        { return hana::eval(static_cast<Then&&>(t)); }

        template <typename Then, typename Else>
        static constexpr decltype(auto)
        apply(decltype(hana::false_c) const&, Then&&, Else&& e)
        { return hana::eval(static_cast<Else&&>(e)); }
    };

    template <typename T>
    struct if_impl<IntegralConstant<T>> {
        template <typename Cond, typename Then, typename Else>
        static constexpr decltype(auto)
        apply(Cond const&, Then&& t, Else&& e) {
            return if_impl::apply(hana::bool_c<static_cast<bool>(Cond::value)>,
                    static_cast<Then&&>(t), static_cast<Else&&>(e));
        }

        //! @todo We could return `Then` instead of `auto` to sometimes save
        //! a copy, but that would break some code that would return a
        //! reference to a Type object. I think the code that would be broken
        //! should be changed, but more thought needs to be given.
        template <typename Then, typename Else>
        static constexpr auto
        apply(decltype(hana::true_c) const&, Then&& t, Else&&)
        { return static_cast<Then&&>(t); }

        template <typename Then, typename Else>
        static constexpr auto
        apply(decltype(hana::false_c) const&, Then&&, Else&& e)
        { return static_cast<Else&&>(e); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOOL_HPP
