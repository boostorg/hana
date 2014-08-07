/*!
@file
Internal header to break cyclic dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_INTEGRAL_FWD_HPP
#define BOOST_HANA_DETAIL_INTEGRAL_FWD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/detail/std/type_traits.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time value of an integral type.
    //!
    //! `Integral`s are guaranteed to have the same members and capabilities
    //! as the corresponding `std::integral_constant`. For example, `Integral`s
    //! are `constexpr`-convertible to their underlying type, they have a
    //! nested static constant named `value` holding their underlying value,
    //! and so on.
    //!
    //! For convenience, common operators are overloaded to return the result
    //! of the corresponding operator as an `integral<...>`.
    //!
    //! ## Overloaded operators
    //! - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`,
    //!                                                      unary `-`
    //! - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    //! - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    //! - Logical: `||`, `&&`, `!`
    //! - Member access: `*` (dereference)
    //!
    //! ## Example
    //! @snippet example/integral/operators.cpp main
    //!
    //! ## Instance of
    //! `Comparable`, `Logical`
    //!
    //! @todo
    //! Implicit conversions to the underlying integral type can be problematic:
    //! @code
    //!     constexpr auto odd = [](auto x) {
    //!         return x % int_<2>;
    //!     };
    //!
    //!     if_(odd(int_<1>), something_of_type_A, something_of_type_B)
    //! @endcode
    //! This will fail because `odd(int_<1>)` has type `Int<1 % 2>`, which is
    //! convertible to `bool` but not to `Bool<...>`. Because of this, the
    //! runtime `if_` is used and compilation fails.
    struct Integral { };

    namespace integral_detail {
        template <typename T, T v>
        struct integral : operators::enable {
            using type = integral;
            using value_type = T;
            static constexpr value_type value = v;
            constexpr operator value_type() const noexcept { return value; }
            constexpr value_type operator()() const noexcept { return value; }

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
    } // end namespace integral_detail

    //! A compile-time integral value of the given type and value.
    //! @relates Integral
    //!
    //! @note
    //! For any `const-volatility` specifier `cv`, `integral<T cv, n>` is
    //! always the same as `integral<T, n>`.
    //!
    //! @internal
    //! ### Rationale for striping cv-specifiers
    //! In the following idiom,
    //! @code
    //!     integral<decltype(Trait::value), Trait::value>
    //! @endcode
    //! if `Trait::value` is declared as `static const T value = ...;`, then
    //! `decltype(Trait::value)` is `T const` instead of `T`. This causes
    //! unintuitive behavior in some cases. For example, with a definition
    //! of `trait::is_floating_point` using the above idiom,
    //! @code
    //!     static_assert(
    //!         decltype_(trait::is_floating_point(type<char>)) ==
    //!         decltype_(false_)
    //!     , "");
    //! @endcode
    //! will fail, but the following will succeed
    //! @code
    //!     static_assert(
    //!         decltype_(trait::is_floating_point(type<char>)) ==
    //!         decltype_(integral<bool const, false>)
    //!     , "");
    //! @endcode
    //! Since there does not seem to be a lot of use cases for keeping
    //! cv-qualifiers in the integral type, we just remove them.
    //!
    //! @todo
    //! Do we want `char_<1> + char_<2> == char_<3>` or
    //! `char_<1> + char_<2> == int_<3>`?
    //!
    //! @todo
    //! Consider specifying the type of `integral<...>` to allow overloading
    //! on its type. Not sure yet whether that's desirable or not.
    template <typename T, T v>
    constexpr integral_detail::integral<
        typename detail::std::remove_cv<T>::type, v
    > integral{};

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
    template <detail::std::size_t i>
    constexpr auto size_t = decltype(integral<detail::std::size_t, i>){};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_INTEGRAL_FWD_HPP
