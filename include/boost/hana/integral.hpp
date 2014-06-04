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


namespace boost { namespace hana {
    /*!
    @datatype{Integral}
    @{
    Represents a compile-time value of an integral type.

    @instantiates{Comparable}

    @todo
    - Document valid expressions with any object of an `Integral` data type as
    in the MPL11?
    - I'm not sure what's the role of `Bool` and friends. I'm also not sure
    of the role of `bool_` and friends. What are they? How should they be
    documented? What is an implementation detail and what's not?
    - Pick another name for this data type; _I is not allowed by the standard.
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
    struct _Integral { };

    //! @}

    template <typename T, T t>
    struct Integral {
        constexpr operator T() const { return t; }
        using hana_datatype = _Integral;
    };

    template <>
    struct Comparable<_Integral, _Integral> : defaults<Comparable> {
        template <typename T, T t, typename U, U u>
        static constexpr Integral<bool, t == u>
        equal_impl(Integral<T, t>, Integral<U, u>)
        { return {}; }

        template <typename T, T t, typename U, U u>
        static constexpr Integral<bool, t != u>
        not_equal_impl(Integral<T, t>, Integral<U, u>)
        { return {}; }
    };

#define BOOST_HANA_INTEGRAL_BINARY_OP(op)                                   \
    template <typename U, U u, typename V, V v>                             \
    constexpr Integral<decltype(u op v), (u op v)>                          \
    operator op(Integral<U, u>, Integral<V, v>)                             \
    { return {}; }                                                          \
/**/
#define BOOST_HANA_INTEGRAL_UNARY_OP(op)                                    \
    template <typename U, U u>                                              \
    constexpr Integral<decltype(op u), (op u)>                              \
    operator op(Integral<U, u>)                                             \
    { return {}; }                                                          \
/**/
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
#undef BOOST_HANA_INTEGRAL_UNARY_OP
#undef BOOST_HANA_INTEGRAL_BINARY_OP

    namespace detail { using std_size_t = decltype(sizeof(int)); }

    template <bool b>
    using Bool = Integral<bool, b>;

    template <char c>
    using Char = Integral<char, c>;

    template <short i>
    using Short = Integral<short, i>;

    template <unsigned short i>
    using UShort = Integral<unsigned short, i>;

    template <int i>
    using Int = Integral<int, i>;

    template <unsigned int i>
    using UInt = Integral<unsigned int, i>;

    template <long i>
    using Long = Integral<long, i>;

    template <unsigned long i>
    using ULong = Integral<unsigned long, i>;

    template <long long i>
    using LLong = Integral<long long, i>;

    template <unsigned long long i>
    using ULLong = Integral<unsigned long long, i>;

    template <detail::std_size_t n>
    using SizeT = Integral<detail::std_size_t, n>;

    template <typename T, T v>
    constexpr Integral<T, v> integral{};

    template <bool b>
    constexpr Bool<b> bool_{};
    constexpr auto true_ = bool_<true>;
    constexpr auto false_ = bool_<false>;

    template <char c>
    constexpr Char<c> char_{};

    template <short i>
    constexpr Short<i> short_{};

    template <unsigned short i>
    constexpr UShort<i> ushort{};

    template <int i>
    constexpr Int<i> int_{};

    template <unsigned int i>
    constexpr UInt<i> uint{};

    template <long i>
    constexpr Long<i> long_{};

    template <unsigned long i>
    constexpr ULong<i> ulong{};

    template <long long i>
    constexpr LLong<i> llong{};

    template <unsigned long long i>
    constexpr ULLong<i> ullong{};

    template <detail::std_size_t n>
    constexpr SizeT<n> size_t{};

    namespace integral_detail {
        constexpr int to_int(char c)
        { return static_cast<int>(c) - 48; }

        template <detail::std_size_t N>
        constexpr unsigned long long parse(const char (&arr)[N]) {
            unsigned long long number = 0, base = 1;
            for (detail::std_size_t i = 0; i < N; ++i) {
                number += to_int(arr[N - 1 - i]) * base;
                base *= 10;
            }
            return number;
        }
    }

    namespace literals {
        //! @todo Add support for stuff like `0x1234_c`.
        template <char ...c>
        constexpr Integral<
            unsigned long long,
            integral_detail::parse<sizeof...(c)>({c...})
        >
        operator"" _c() { return {}; }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
