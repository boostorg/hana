/*!
@file
Forward declares `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGRAL_HPP
#define BOOST_HANA_FWD_INTEGRAL_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/n_times.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/remove_cv.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/fwd/comparable.hpp>
#include <boost/hana/fwd/group.hpp>
#include <boost/hana/fwd/integral_domain.hpp>
#include <boost/hana/fwd/logical.hpp>
#include <boost/hana/fwd/monoid.hpp>
#include <boost/hana/fwd/orderable.hpp>
#include <boost/hana/fwd/ring.hpp>


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
    //! `Integral`s also have a member function named `times`, which allows a
    //! nullary function to be invoked `n` times.
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
    //! @snippet example/integral.cpp operators
    //!
    //! ## Instance of
    //! `Comparable`, `Orderable`, `Constant`, `Logical`, `Monoid`, `Group`,
    //! `Ring`, `IntegralDomain`, and `IntegralConstant`.
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
    //!
    //! @todo
    //! `times` should be a shortcut to some type class method. Specifically,
    //! anything that can be incremented, decremented and compared to some
    //! "zero" value can implement a `times` method with the same semantics.
    struct Integral {
        struct hana_enabled_operators
            : Comparable, Orderable,
              Logical,
              Monoid, Group, Ring, IntegralDomain
        { };
    };

    namespace integral_detail {
        template <typename T, T v>
        struct integral : operators::enable_adl {
            using type = integral;
            using value_type = T;
            static constexpr value_type value = v;
            constexpr operator value_type() const noexcept { return value; }
            constexpr value_type operator()() const noexcept { return value; }

            struct hana { using datatype = Integral; };

            template <typename F>
            constexpr void times(F&& f) const {
                detail::n_times<v>(detail::std::forward<F>(f));
            }
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

    namespace integral_detail {
        constexpr int to_int(char c)
        { return static_cast<int>(c) - 48; }

        template <detail::std::size_t N>
        constexpr long long parse(const char (&arr)[N]) {
            long long number = 0, base = 1;
            for (decltype(N) i = 0; i < N; ++i) {
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
        //! @snippet example/integral.cpp literals
        //!
        //! @todo Add support for stuff like `0x1234_c`.
        template <char ...c>
        constexpr auto operator"" _c()
        { return llong<integral_detail::parse<sizeof...(c)>({c...})>; }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGRAL_HPP
