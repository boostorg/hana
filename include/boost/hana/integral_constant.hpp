/*!
@file
Defines `boost::hana::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_INTEGRAL_CONSTANT_HPP

#include <boost/hana/fwd/integral_constant.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/detail/std/size_t.hpp>

// provided models
#include <boost/hana/comparable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // integral_constant
    //////////////////////////////////////////////////////////////////////////
    namespace ic_detail {
        template <typename T, T N, typename = detail::std::make_integer_sequence<T, N>>
        struct go;

        template <typename T, T N, T ...i>
        struct go<T, N, detail::std::integer_sequence<T, i...>> {
            using swallow = T[];

            template <typename F>
            static constexpr void with_index(F&& f)
            { (void)swallow{T{}, ((void)f(integral_constant<T, i>), i)...}; }

            template <typename F>
            static constexpr void without_index(F&& f)
            { (void)swallow{T{}, ((void)f(), i)...}; }
        };

        template <typename T, T v>
        struct _with_index {
            template <typename F>
            constexpr void operator()(F&& f) const
            { go<T, (sizeof(F), v)>::with_index(detail::std::forward<F>(f)); }
        };

        template <typename T, T v>
        struct _times {
            static constexpr _with_index<T, v> with_index{};

            template <typename F>
            constexpr void operator()(F&& f) const
            { go<T, (sizeof(F), v)>::without_index(detail::std::forward<F>(f)); }
        };
    }

    template <typename T, T v>
    struct _integral_constant : operators::enable_adl {
        // std::integral_constant interface
        using type = _integral_constant;
        using value_type = T;
        static constexpr value_type value = v;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }

        // times
        static constexpr ic_detail::_times<T, v> times{};
    };

    template <typename T, T const v>
    struct _integral_constant<T const, v>
        : _integral_constant<T, v>
    { };

    template <typename T, T volatile v>
    struct _integral_constant<T volatile, v>
        : _integral_constant<T, v>
    { };

    template <typename T, T const volatile v>
    struct _integral_constant<T const volatile, v>
        : _integral_constant<T, v>
    { };

    template <typename T, T v>
    struct datatype<_integral_constant<T, v>> {
        using type = IntegralConstant<T>;
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators (most of them are provided by the concepts)
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct enabled_operators<IntegralConstant<T>>
        : Comparable, Orderable
        , Logical
        , Monoid, Group, Ring, IntegralDomain
    { };

#define BOOST_HANA_INTEGRAL_CONSTANT_BINARY_OP(op)                          \
    template <typename U, U u, typename V, V v>                             \
    constexpr _integral_constant<decltype(u op v), (u op v)>                \
    operator op(_integral_constant<U, u>, _integral_constant<V, v>)         \
    { return {}; }                                                          \
    /**/

#define BOOST_HANA_INTEGRAL_CONSTANT_UNARY_OP(op)                           \
    template <typename U, U u>                                              \
    constexpr _integral_constant<decltype(op u), (op u)>                    \
    operator op(_integral_constant<U, u>)                                   \
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

        template <detail::std::size_t N>
        constexpr long long parse(const char (&arr)[N]) {
            long long number = 0, base = 1;
            for (detail::std::size_t i = 0; i < N; ++i) {
                number += to_int(arr[N - 1 - i]) * base;
                base *= 10;
            }
            return number;
        }
    }

    template <char ...c>
    constexpr auto literals::operator"" _c()
    { return llong<ic_detail::parse<sizeof...(c)>({c...})>; }

    //////////////////////////////////////////////////////////////////////////
    // Model of Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(IntegralConstant<T>)>
        : detail::std::true_type
    { };

    template <typename T>
    struct value_impl<IntegralConstant<T>> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return C::value; }
    };

    template <typename T, typename C>
    struct convert<IntegralConstant<T>, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding_if<is_embedding<convert<T, typename C::value_type>>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to an IntegralConstant of a non-integral "
        "type; boost::hana::IntegralConstant may only hold integral types");

        template <typename X>
        static constexpr auto apply(X)
        { return integral_constant<T, value2<X>()>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_CONSTANT_HPP
