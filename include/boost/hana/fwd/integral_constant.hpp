/*!
@file
Forward declares `boost::hana::integral_constant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP

#include <boost/hana/detail/operators/adl.hpp>

#include <cstddef>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Compile-time value of an integral type.
    //!
    //! An `integral_constant` is an object that represents a compile-time
    //! integral value. As the name suggests, `hana::integral_constant` is
    //! basically equivalent to `std::integral_constant`, except that
    //! `hana::integral_constant` also provide other goodies to make them
    //! easier to use, like arithmetic operators and similar features. In
    //! particular, they are guaranteed to have the same members and
    //! capabilities as `std::integral_constant`s. The sections below
    //! explain the extensions to `std::integral_constant` provided by
    //! `hana::integral_constant`.
    //!
    //!
    //! Arithmetic operators
    //! --------------------
    //! `hana::integral_constant` provides arithmetic operators that return
    //! `hana::integral_constant`s to ease writing compile-time arithmetic:
    //! @snippet example/integral_constant.cpp operators
    //!
    //! It is pretty important to realize that these operators return other
    //! `integral_constant`s, not normal values of an integral type.
    //! Actually, all those operators work pretty much in the same way.
    //! Simply put, for an operator `@`,
    //! @code
    //!     integral_constant<T, x>{} @ integral_constant<T, y>{} == integral_constant<T, x @ y>{}
    //! @endcode
    //!
    //! The fact that the operators return `Constant`s is very important
    //! because it allows all the information that's known at compile-time
    //! to be conserved as long as it's only used with other values known at
    //! compile-time. It is also interesting to observe that whenever an
    //! `integral_constant` is combined with a normal runtime value, the
    //! result will be a runtime value (because of the implicit conversion).
    //! In general, this gives us the following table
    //!
    //! left operand        | right operand       | result
    //! :-----------------: | :-----------------: | :-----------------:
    //! `integral_constant` | `integral_constant` | `integral_constant`
    //! `integral_constant` | runtime             | runtime
    //! runtime             | `integral_constant` | runtime
    //! runtime             | runtime             | runtime
    //!
    //! The full range of provided operators is
    //! - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    //! - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    //! - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    //! - %Logical: `||`, `&&`, `!`
    //! - Member access: `*` (dereference)
    //!
    //!
    //! `times` function
    //! ----------------
    //! `hana::integral_constant`s also have a static member function object
    //! named `times`, which allows a nullary function to be invoked `n` times:
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
    //! from the type of an `integral_constant` object and from an object
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
    //! the indices passed to the function are `integral_constant`s; they are
    //! known at compile-time. Hence, we can do compile-time stuff with them,
    //! like indexing inside a tuple:
    //! @snippet example/integral_constant.cpp times_with_index_compile_time
    //!
    //! @note
    //! `times.with_index(f)` guarantees that the calls to `f` will be done in
    //! order of ascending index. In other words, `f` will be called as `f(0)`,
    //! `f(1)`, `f(2)`, etc., but with `integral_constant`s instead of normal
    //! integers. Side effects can also be done in the function passed to
    //! `.times` and `.times.with_index`.
    //!
    //!
    //! Construction with user-defined literals
    //! ---------------------------------------
    //! `integral_constant`s of type `long long` can be created with the
    //! `_c` user-defined literal, which is contained in the `literals`
    //! namespace:
    //! @snippet example/integral_constant.cpp literals
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Constant`\n
    //! An `integral_constant` is a model of the `Constant` concept in the
    //! most obvious way possible. Specifically,
    //! @code
    //!     constexpr auto c = integral_c<int, 10>;
    //!     static_assert(value2<decltype(c)>() == 10, "");
    //! @endcode
    //!
    //! 2. `Enumerable`, `Comparable`, `Orderable`, `Logical`, `Monoid`,
    //!    `Group`, `Ring`, and `IntegralDomain`\n
    //! Those models are exactly those provided for `Constant`s, which are
    //! documented in their respective concepts.
    template <typename T, T v>
    struct integral_constant;

    //! Tag representing `hana::integral_constant`.
    //! @relates hana::integral_constant
    template <typename T>
    struct IntegralConstant {
        using value_type = T;
    };

    namespace ic_detail {
        template <typename T, T v>
        struct with_index_t {
            template <typename F>
            constexpr void operator()(F&& f) const;
        };

        template <typename T, T v>
        struct times_t {
            static constexpr with_index_t<T, v> with_index{};

            template <typename F>
            constexpr void operator()(F&& f) const;
        };
    }

    //! @cond
    template <typename T, T v>
    struct integral_constant : operators::adl {
        // std::integral_constant interface
        using type = integral_constant;
        using value_type = T;
        static constexpr value_type value = v;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }

        // times
        static constexpr ic_detail::times_t<T, v> times{};

        using hana = integral_constant;
        using datatype = IntegralConstant<T>;
    };
    //! @endcond

    template <bool b>
    using _bool = integral_constant<bool, b>;

    using _true = _bool<true>;

    using _false = _bool<false>;

    template <char c>
    using _char = integral_constant<char, c>;

    template <short i>
    using _short = integral_constant<short, i>;

    template <unsigned short i>
    using _ushort = integral_constant<unsigned short, i>;

    template <int i>
    using _int = integral_constant<int, i>;

    template <unsigned int i>
    using _uint = integral_constant<unsigned int, i>;

    template <long i>
    using _long = integral_constant<long, i>;

    template <unsigned long i>
    using _ulong = integral_constant<unsigned long, i>;

    template <long long i>
    using _llong = integral_constant<long long, i>;

    template <unsigned long long i>
    using _ullong = integral_constant<unsigned long long, i>;

    template <std::size_t i>
    using _size_t = integral_constant<std::size_t, i>;

    //! Creates an `integral_constant` holding the given compile-time value.
    //! @relates hana::integral_constant
    //!
    //! Specifically, `integral_c<T, v>` is a `hana::integral_constant`
    //! holding the compile-time value `v` of an integral type `T`.
    //!
    //!
    //! @tparam T
    //! The type of the value to hold in the `integral_constant`.
    //! It must be an integral type.
    //!
    //! @tparam v
    //! The integral value to hold in the `integral_constant`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/integral_constant.cpp integral_c
    template <typename T, T v>
    constexpr integral_constant<T, v> integral_c{};

    //! @relates hana::integral_constant
    template <bool b>
    constexpr integral_constant<bool, b> bool_{};

    //! Equivalent to `bool_<true>`.
    //! @relates hana::integral_constant
    constexpr auto true_ = bool_<true>;

    //! Equivalent to `bool_<false>`.
    //! @relates hana::integral_constant
    constexpr auto false_ = bool_<false>;

    //! @relates hana::integral_constant
    template <char c>
    constexpr integral_constant<char, c> char_{};

    //! @relates hana::integral_constant
    template <short i>
    constexpr integral_constant<short, i> short_{};

    //! @relates hana::integral_constant
    template <unsigned short i>
    constexpr integral_constant<unsigned short, i> ushort{};

    //! @relates hana::integral_constant
    template <int i>
    constexpr integral_constant<int, i> int_{};

    //! @relates hana::integral_constant
    template <unsigned int i>
    constexpr integral_constant<unsigned int, i> uint{};

    //! @relates hana::integral_constant
    template <long i>
    constexpr integral_constant<long, i> long_{};

    //! @relates hana::integral_constant
    template <unsigned long i>
    constexpr integral_constant<unsigned long, i> ulong{};

    //! @relates hana::integral_constant
    template <long long i>
    constexpr integral_constant<long long, i> llong{};

    //! @relates hana::integral_constant
    template <unsigned long long i>
    constexpr integral_constant<unsigned long long, i> ullong{};

    //! @relates hana::integral_constant
    template <std::size_t i>
    constexpr integral_constant<std::size_t, i> size_t{};

    namespace literals {
        //! Creates a `hana::integral_constant` from a literal.
        //! @relatesalso boost::hana::integral_constant
        //!
        //! The literal is parsed at compile-time and the result is returned
        //! as a `llong<...>`.
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
        template <char ...c>
        constexpr auto operator"" _c();
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_INTEGRAL_CONSTANT_HPP
