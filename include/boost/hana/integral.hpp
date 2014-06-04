/*!
@file
Defines `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_HPP
#define BOOST_HANA_INTEGRAL_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>

#include <cstddef>


namespace boost { namespace hana {
    /*!
    @datatype{Integral}
    @{
    Represents a compile-time value of an integral type.

    Let `n` be an object of an `Integral` data type. The compile-time value
    represented by `n` is accessible as `value(n)`, which is a `constexpr`
    object of the underlying integral type. `n` is also implicitly
    `constexpr`-convertible to the underlying integral type.

    @instantiates{Comparable}

    @todo
    - Do we want `char_<1> + char<2> == char_<3>` or
      `char_<1> + char_<2> == int_<3>`?
    - Conversions from `Integral<T, t>` to `T` are sometimes problematic.
    Consider:
    @code
        constexpr auto odd = [](auto x) {
            return x % int_<2>;
        };

        if_(odd(int_<1>), something_of_type_A, something_of_type_B)
    @endcode
    This will fail because `odd(int_<1>)` has type `Int<1 % 2>`, which is
    convertible to `bool` but not to `Bool<...>`. Because of this, the
    runtime `if_` is used and compilation fails.
     */
    struct Integral { };

    //! @}

    namespace integral_detail {
        template <typename T, T t>
        struct integral_type {
            // See test/integral/bug_constexpr_value.cpp for why we use
            // a const& here.
            friend constexpr T value(integral_type const&) { return t; }
            constexpr operator T() const { return t; }
            using hana_datatype = Integral;
        };
    }

    template <typename T, T v>
    constexpr integral_detail::integral_type<T, v> integral{};

    template <bool b>
    constexpr decltype(integral<bool, b>) bool_{};
    constexpr auto true_ = bool_<true>;
    constexpr auto false_ = bool_<false>;

    template <char c>
    constexpr decltype(integral<char, c>) char_;

    template <short i>
    constexpr decltype(integral<short, i>) short_{};

    template <unsigned short i>
    constexpr decltype(integral<unsigned short, i>) ushort{};

    template <int i>
    constexpr decltype(integral<int, i>) int_{};

    template <unsigned int i>
    constexpr decltype(integral<unsigned int, i>) uint{};

    template <long i>
    constexpr decltype(integral<long, i>) long_{};

    template <unsigned long i>
    constexpr decltype(integral<unsigned long, i>) ulong{};

    template <long long i>
    constexpr decltype(integral<long long, i>) llong{};

    template <unsigned long long i>
    constexpr decltype(integral<unsigned long long, i>) ullong{};

    template <std::size_t i>
    constexpr decltype(integral<std::size_t, i>) size_t{};

    template <>
    struct Comparable<Integral, Integral> : defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(T t, U u)
        { return bool_<value(t) == value(u)>; }
    };

#define BOOST_HANA_INTEGRAL_BINARY_OP(op)                                   \
    template <typename U, U u, typename V, V v>                             \
    constexpr integral_type<decltype(u op v), (u op v)>                     \
    operator op(integral_type<U, u>, integral_type<V, v>)                   \
    { return {}; }                                                          \
/**/
#define BOOST_HANA_INTEGRAL_UNARY_OP(op)                                    \
    template <typename U, U u>                                              \
    constexpr integral_type<decltype(op u), (op u)>                         \
    operator op(integral_type<U, u>)                                        \
    { return {}; }                                                          \
/**/
    namespace integral_detail {
        // Arithmetic
        BOOST_HANA_INTEGRAL_UNARY_OP(+)
        BOOST_HANA_INTEGRAL_UNARY_OP(-)
        BOOST_HANA_INTEGRAL_BINARY_OP(+)
        BOOST_HANA_INTEGRAL_BINARY_OP(-)
        BOOST_HANA_INTEGRAL_BINARY_OP(*)
        BOOST_HANA_INTEGRAL_BINARY_OP(/)
        BOOST_HANA_INTEGRAL_BINARY_OP(%)
        BOOST_HANA_INTEGRAL_UNARY_OP(~)
        BOOST_HANA_INTEGRAL_BINARY_OP(&)
        BOOST_HANA_INTEGRAL_BINARY_OP(|)
        BOOST_HANA_INTEGRAL_BINARY_OP(^)
        BOOST_HANA_INTEGRAL_BINARY_OP(<<)
        BOOST_HANA_INTEGRAL_BINARY_OP(>>)

        // Comparison
        BOOST_HANA_INTEGRAL_BINARY_OP(==)
        BOOST_HANA_INTEGRAL_BINARY_OP(!=)
        BOOST_HANA_INTEGRAL_BINARY_OP(<)
        BOOST_HANA_INTEGRAL_BINARY_OP(<=)
        BOOST_HANA_INTEGRAL_BINARY_OP(>)
        BOOST_HANA_INTEGRAL_BINARY_OP(>=)

        // Logical
        BOOST_HANA_INTEGRAL_BINARY_OP(||)
        BOOST_HANA_INTEGRAL_BINARY_OP(&&)
        BOOST_HANA_INTEGRAL_UNARY_OP(!)
    }
#undef BOOST_HANA_INTEGRAL_UNARY_OP
#undef BOOST_HANA_INTEGRAL_BINARY_OP

    namespace integral_detail {
        constexpr int to_int(char c)
        { return static_cast<int>(c) - 48; }

        template <std::size_t N>
        constexpr unsigned long long parse(const char (&arr)[N]) {
            unsigned long long number = 0, base = 1;
            for (std::size_t i = 0; i < N; ++i) {
                number += to_int(arr[N - 1 - i]) * base;
                base *= 10;
            }
            return number;
        }
    }

    namespace literals {
        //! @todo Add support for stuff like `0x1234_c`.
        template <char ...c>
        constexpr auto operator"" _c()
        { return ullong<integral_detail::parse<sizeof...(c)>({c...})>; }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
