/*!
@file
Defines `boost::hana::IntegralConstant` and includes `Logical` and
`Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOOL_HPP
#define BOOST_HANA_BOOL_HPP

#include <boost/hana/fwd/bool.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/is_integral.hpp>
#include <boost/hana/detail/std/size_t.hpp>

// provided models; the rest is included in <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/comparable.hpp>

// operators
#include <boost/hana/fwd/group.hpp>
#include <boost/hana/fwd/integral_domain.hpp>
#include <boost/hana/fwd/monoid.hpp>
#include <boost/hana/fwd/orderable.hpp>
#include <boost/hana/fwd/ring.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // integral_constant
    //////////////////////////////////////////////////////////////////////////
    //! @cond
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
        template <typename F>
        constexpr void _with_index<T, v>::operator()(F&& f) const
        { go<T, (sizeof(F), v)>::with_index(detail::std::forward<F>(f)); }

        template <typename T, T v>
        template <typename F>
        constexpr void _times<T, v>::operator()(F&& f) const
        { go<T, (sizeof(F), v)>::without_index(detail::std::forward<F>(f)); }

        // avoid link-time error
        template <typename T, T v>
        constexpr _with_index<T, v> _times<T, v>::with_index;
    }

    // avoid link-time error
    template <typename T, T v>
    constexpr ic_detail::_times<T, v> _integral_constant<T, v>::times;

    template <typename T, T v>
    struct datatype<_integral_constant<T, v>> {
        using type = IntegralConstant<T>;
    };
    //! @endcond

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

    namespace literals {
        template <char ...c>
        constexpr auto operator"" _c()
        { return llong<ic_detail::parse<sizeof...(c)>({c...})>; }
    }

    //////////////////////////////////////////////////////////////////////////
    // Model of Constant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct value_impl<IntegralConstant<T>> {
        template <typename C>
        static constexpr auto apply(C const&)
        { return C::value; }
    };

    template <typename T, typename C>
    struct to_impl<IntegralConstant<T>, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, T>{}>
    {
        static_assert(detail::std::is_integral<T>{},
        "trying to convert a Constant to an IntegralConstant of a non-integral "
        "type; boost::hana::IntegralConstant may only hold integral types");

        template <typename X>
        static constexpr auto apply(X x) {
            constexpr auto v = hana::value(x);
            return integral_constant<T, static_cast<T>(v)>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOOL_HPP
