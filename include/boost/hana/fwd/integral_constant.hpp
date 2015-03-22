/*!
@file
Forward declares `boost::hana::IntegralConstant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a compile-time value of an integral type.
    //!
    //! An `IntegralConstant` is an object that represents a compile-time
    //! integral value. In fact, as the name suggests, an `IntegralConstant`
    //! is basically equivalent to a `std::integral_constant`, except that
    //! Hana's `IntegralConstant`s also provide other goodies to make them
    //! easier to use. In particular, `IntegralConstant`s are guaranteed to
    //! have the same members and capabilities as `std::integral_constant`s.
    //! For example, `IntegralConstant`s are `constexpr`-convertible to their
    //! underlying type, they have a nested static constant named `value`
    //! holding their underlying value, and so on.
    //!
    //! > #### Rationale for not sticking with `std::integral_constant`
    //! > First, we wanted to have arithmetic operators and other goodies,
    //! > which is much harder if we don't use our own objects. Second, using
    //! > `std::integral_constant` requires including the whole `<type_traits>`
    //! > header, which is about half the size of the whole Hana library.
    //!
    //!
    //! As outlined above, the interface of an `IntegralConstant` is a
    //! superset of what's provided by `std::integral_constant`. The sections
    //! below explain those additions.
    //!
    //!
    //! Arithmetic operators
    //! --------------------
    //! `IntegralConstant` provides arithmetic operators that return
    //! `IntegralConstant`s, so compile-time arithmetic can be done easily:
    //! @snippet example/integral_constant.cpp operators
    //!
    //! It is pretty important to realize that these operators return
    //! `IntegralConstant`s, not normal values of an integral type.
    //! Actually, all those operators work pretty much in the same way.
    //! Simply put, for an operator `@`,
    //! @code
    //!     integral_constant<T, x> @ integral_constant<T, y> == integral_constant<T, x @ y>
    //! @endcode
    //!
    //! The fact that the operators return `Constant`s is very important
    //! because it allows all the information that's known at compile-time
    //! to be conserved as long as it's only used with other values known at
    //! compile-time. It is also interesting to observe that whenever an
    //! `IntegralConstant` is combined with a normal runtime value, the
    //! result will be a runtime value (because of the implicit conversion).
    //! In general, this gives us the following table
    //!
    //! left operand       | right operand      | result
    //! :-----------------:| :----------------: | :----------------:
    //! `IntegralConstant` | `IntegralConstant` | `IntegralConstant`
    //! `IntegralConstant` | runtime            | runtime
    //! runtime            | `IntegralConstant` | runtime
    //! runtime            | runtime            | runtime
    //!
    //! The full range of provided operators is
    //! - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    //! - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    //! - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    //! - Logical: `||`, `&&`, `!`
    //! - Member access: `*` (dereference)
    //!
    //!
    //! `times` function
    //! ----------------
    //! `IntegralConstant`s also have a static member function object named
    //! `times`, which allows a nullary function to be invoked `n` times:
    //! @code
    //!     int_<3>.times(f)
    //! @endcode
    //! should be expanded by any decent compiler to
    //! @code
    //!     f(); f(); f();
    //! @endcode
    //!
    //! This can be useful in several contexts, e.g. for loop unrolling:
    //! @snippet example/integral_constant.cpp times_loop_unrolling
    //!
    //! Note that `times` is really a static function object. Since static
    //! members can be accessed using both the `.` and the `::` syntax, Hana
    //! takes advantage of this (loophole?) to make `times` accessible both
    //! from the type of an `IntegralConstant` object and from an object
    //! itself:
    //! @snippet example/integral_constant.cpp as_static_member
    //!
    //! Also, since `times` is a function object instead of an
    //! overloaded function, it does not need to be called right away, which
    //! can be useful in conjunction with some higher-order algorithms:
    //! @snippet example/integral_constant.cpp times_higher_order
    //!
    //! Sometimes, it is also useful to know the index we're at inside the
    //! function. This is also possible:
    //! @snippet example/integral_constant.cpp times_with_index_runtime
    //!
    //! Remember that `times` is a _function object_, and hence it can have
    //! subobjects. `with_index` is then just a function object nested inside
    //! `times`, which allows for this nice little interface. Also note that
    //! the indices passed to the function are `IntegralConstant`s; they are
    //! known at compile-time. Hence, we can do compile-time stuff with them,
    //! like indexing inside a tuple:
    //! @snippet example/integral_constant.cpp times_with_index_compile_time
    //!
    //!
    //! Construction with user-defined literals
    //! ---------------------------------------
    //! `IntegralConstant`s of type `long long` can be created with the
    //! `_c` user-defined literal, which is contained in the `literals`
    //! namespace:
    //! @snippet example/integral_constant.cpp literals
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Constant`\n
    //! An `IntegralConstant` is a model of the `Constant` concept in the most
    //! obvious way possible. Specifically,
    //! @code
    //!     constexpr auto c = integral_constant<int, 10>;
    //!     static_assert(value2<decltype(c)>() == 10, "");
    //! @endcode
    //!
    //! 2. `Comparable`, `Orderable`, `Logical`, `Monoid`, `Group`, `Ring`,
    //!    and `IntegralDomain` (operators provided)\n
    //! Those models are exactly those provided for `Constant`s, which are
    //! documented in their respective concepts.
    //!
    //!
    //! @todo
    //! `times` should be a shortcut to some tag-dispatched method. Specifically,
    //! anything that can be incremented, decremented and compared to some
    //! "zero" value can implement a `times` method with the same semantics.
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
    template <typename T>
    struct IntegralConstant {
        using value_type = T;
    };

    namespace ic_detail {
        template <typename T, T v>
        struct _with_index {
            template <typename F>
            constexpr void operator()(F&& f) const;
        };

        template <typename T, T v>
        struct _times {
            static constexpr _with_index<T, v> with_index{};

            template <typename F>
            constexpr void operator()(F&& f) const;
        };
    }

    template <typename T, T v>
    struct _integral_constant : operators::adl {
        // std::integral_constant interface
        using type = _integral_constant;
        using value_type = T;
        static constexpr value_type value = v;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }

        // times
        static constexpr ic_detail::_times<T, v> times{};

        using hana = _integral_constant;
        using datatype = IntegralConstant<T>;
    };

    //! Creates an `IntegralConstant` holding the given compile-time value.
    //! @relates IntegralConstant
    //!
    //! Specifically, `integral_constant<T, v` is an `IntegralConstant`
    //! holding the compile-time value `v` of an integral type `T`.
    //!
    //!
    //! @tparam T
    //! The type of the value to hold in the `IntegralConstant`. It must be an
    //! integral type.
    //!
    //! @tparam v
    //! The integral value to hold in the `IntegralConstant`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/integral_constant.cpp integral_constant
    template <typename T, T v>
    constexpr _integral_constant<T, v> integral_constant{};

    //! @relates IntegralConstant
    template <bool b>
    constexpr _integral_constant<bool, b> bool_{};

    //! Equivalent to `bool_<true>`.
    //! @relates IntegralConstant
    constexpr auto true_ = bool_<true>;

    //! Equivalent to `bool_<false>`.
    //! @relates IntegralConstant
    constexpr auto false_ = bool_<false>;

    //! @relates IntegralConstant
    template <char c>
    constexpr _integral_constant<char, c> char_{};

    //! @relates IntegralConstant
    template <short i>
    constexpr _integral_constant<short, i> short_{};

    //! @relates IntegralConstant
    template <unsigned short i>
    constexpr _integral_constant<unsigned short, i> ushort{};

    //! @relates IntegralConstant
    template <int i>
    constexpr _integral_constant<int, i> int_{};

    //! @relates IntegralConstant
    template <unsigned int i>
    constexpr _integral_constant<unsigned int, i> uint{};

    //! @relates IntegralConstant
    template <long i>
    constexpr _integral_constant<long, i> long_{};

    //! @relates IntegralConstant
    template <unsigned long i>
    constexpr _integral_constant<unsigned long, i> ulong{};

    //! @relates IntegralConstant
    template <long long i>
    constexpr _integral_constant<long long, i> llong{};

    //! @relates IntegralConstant
    template <unsigned long long i>
    constexpr _integral_constant<unsigned long long, i> ullong{};

    //! @relates IntegralConstant
    template <detail::std::size_t i>
    constexpr _integral_constant<detail::std::size_t, i> size_t{};

    namespace literals {
        //! Creates an `IntegralConstant` from a literal.
        //! @relates boost::hana::IntegralConstant
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
        //!
        //! Example
        //! -------
        //! @snippet example/integral_constant.cpp literals
        //!
        //! @todo Add support for stuff like `0x1234_c`.
        template <char ...c>
        constexpr auto operator"" _c();
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP
