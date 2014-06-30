/*!
@file
Internal header to break cyclic dependencies.

@todo
Some headers include this only, which breaks them if they don't include
the full `boost/hana/integral.hpp` and return an `Integral` from one of
their function (the user would expect that the header includes everything
that's required). Because of circular dependencies, those headers can't
include `boost/hana/integral.hpp`. Fix this.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_INTEGRAL_FWD_HPP
#define BOOST_HANA_DETAIL_INTEGRAL_FWD_HPP

#include <cstddef>


namespace boost { namespace hana {
    /*!
    @ingroup datatypes
    Represents a compile-time value of an integral type.

    Let `n` be an object of an `Integral` data type. The compile-time value
    represented by `n` is accessible as `n()`, which is a `constexpr`
    object of the underlying integral type. `n` is also implicitly
    `constexpr`-convertible to the underlying integral type.

    ## Instance of
    `Comparable`, `Logical`

    --------------------------------------------------------------------------

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

    namespace operators {
        template <typename T, T t>
        struct integral {
            constexpr T operator()() const { return t; }
            constexpr operator T() const { return t; }
            using hana_datatype = Integral;
        };

#define BOOST_HANA_INTEGRAL_BINARY_OP(op)                                   \
    template <typename U, U u, typename V, V v>                             \
    constexpr integral<decltype(u op v), (u op v)>                          \
    operator op(integral<U, u>, integral<V, v>)                             \
    { return {}; }                                                          \
/**/

#define BOOST_HANA_INTEGRAL_UNARY_OP(op)                                    \
    template <typename U, U u>                                              \
    constexpr integral<decltype(op u), (op u)>                              \
    operator op(integral<U, u>)                                             \
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

        // Bitwise
        BOOST_HANA_INTEGRAL_UNARY_OP(~)
        BOOST_HANA_INTEGRAL_BINARY_OP(&)
        BOOST_HANA_INTEGRAL_BINARY_OP(|)
        BOOST_HANA_INTEGRAL_BINARY_OP(^)
        BOOST_HANA_INTEGRAL_BINARY_OP(<<)
        BOOST_HANA_INTEGRAL_BINARY_OP(>>)

#undef BOOST_HANA_INTEGRAL_UNARY_OP
#undef BOOST_HANA_INTEGRAL_BINARY_OP
    }

    namespace integral_detail {
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
    constexpr operators::integral<
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_INTEGRAL_FWD_HPP
