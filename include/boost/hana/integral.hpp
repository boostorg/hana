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
    Represents a compile-time value of an integral type.

    Let `n` be an object of an `Integral` data type. The compile-time value
    represented by `n` is accessible as `value(n)`, which is a `constexpr`
    object of the underlying integral type. `n` is also implicitly
    `constexpr`-convertible to the underlying integral type.

    @instantiates{Comparable}

    @todo
    Implicit conversions to the underlying integral type can be problematic:
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

    namespace integral_detail {
        template <typename T, T t>
        struct integral_type {
            // See test/integral/bug_constexpr_value.cpp for why we use
            // a const& here.
            friend constexpr T value(integral_type const&) { return t; }
            constexpr operator T() const { return t; }
            using hana_datatype = Integral;
        };

        // We're not gonna include the whole <type_traits> for this.
        template <typename T>
        struct remove_cv { using type = T; };

        template <typename T>
        struct remove_cv<T const> { using type = T; };

        template <typename T>
        struct remove_cv<T volatile> { using type = T; };

        template <typename T>
        struct remove_cv<T const volatile> { using type = T; };
    }

    /*!
    A compile-time integral value of the given type and value.
    @relates Integral

    For convenience, common operators are overloaded to return the result
    of the corresponding operator as an `integral<...>`.

    ### Overloaded operators
    - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    - Logical: `||`, `&&`, `!`
    - Member access: `*` (dereference)

    ### Example
    @snippet example/integral/operators.cpp main

    @note
    For any `const-volatility` specifier `cv`, `integral<T cv, n>` is always
    the same as `integral<T, n>`.

    @internal
    ### Rationale for striping cv-specifiers
    In the following idiom,
    @code
        integral<decltype(Trait::value), Trait::value>
    @endcode
    if `Trait::value` is declared as `static const T value = ...;`, then
    `decltype(Trait::value)` is `T const` instead of `T`. This causes
    unintuitive behavior in some cases. For example, with a definition
    of `trait::is_floating_point` using the above idiom,
    @code
        static_assert(
            decltype_(trait::is_floating_point(type<char>)) ==
            decltype_(false_)
        , "");
    @endcode
    will fail, but the following will succeed
    @code
        static_assert(
            decltype_(trait::is_floating_point(type<char>)) ==
            decltype_(integral<bool const, false>)
        , "");
    @endcode
    Since there does not seem to be a lot of use cases for keeping
    cv-qualifiers in the integral type, we just remove them.

    @todo
    Do we want `char_<1> + char_<2> == char_<3>` or `char_<1> + char_<2> == int_<3>`?
     */
    template <typename T, T v>
    constexpr integral_detail::integral_type<
              typename integral_detail::remove_cv<T>::type, v> integral{};

    //! @relates Integral
    template <bool b>
    constexpr auto bool_ = decltype(integral<bool, b>){};

    //! @relates Integral
    constexpr auto true_ = bool_<true>;

    //! @relates Integral
    constexpr auto false_ = bool_<false>;

    //! @relates Integral
    template <char c>
    constexpr auto char_ = decltype(integral<char, c>){};

    //! @relates Integral
    template <short i>
    constexpr auto short_ = decltype(integral<short, i>){};

    //! @relates Integral
    template <unsigned short i>
    constexpr auto ushort = decltype(integral<unsigned short, i>){};

    //! @relates Integral
    template <int i>
    constexpr auto int_ = decltype(integral<int, i>){};

    //! @relates Integral
    template <unsigned int i>
    constexpr auto uint = decltype(integral<unsigned int, i>){};

    //! @relates Integral
    template <long i>
    constexpr auto long_ = decltype(integral<long, i>){};

    //! @relates Integral
    template <unsigned long i>
    constexpr auto ulong = decltype(integral<unsigned long, i>){};

    //! @relates Integral
    template <long long i>
    constexpr auto llong = decltype(integral<long long, i>){};

    //! @relates Integral
    template <unsigned long long i>
    constexpr auto ullong = decltype(integral<unsigned long long, i>){};

    //! @relates Integral
    template <std::size_t i>
    constexpr auto size_t = decltype(integral<std::size_t, i>){};

    template <>
    struct Comparable<Integral, Integral>
        : defaults<Comparable>::with<Integral, Integral>
    {
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
        //! Creates an `Integral` from a literal.
        //! @relates boost::hana::Integral
        //!
        //! The literal is parsed at compile-time and the result is returned
        //! as an `llong<...>`.
        //!
        //! @note
        //! We use `llong<...>` instead of `ullong<...>` because using an
        //! unsigned type leads to unexpected behavior when doing stuff like
        //! `-1_c`. If we used an unsigned type, `-1_c` would be something
        //! like `ullong<-1>` which is actually `ullong<something huge>`.
        //!
        //! ### Example
        //! @snippet example/integral/literals.cpp main
        //!
        //! @todo Add support for stuff like `0x1234_c`.
        template <char ...c>
        constexpr auto operator"" _c()
        { return llong<integral_detail::parse<sizeof...(c)>({c...})>; }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
