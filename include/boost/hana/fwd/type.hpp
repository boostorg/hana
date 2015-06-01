/*!
@file
Forward declares `boost::hana::Type` and `boost::hana::Metafunction`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TYPE_HPP
#define BOOST_HANA_FWD_TYPE_HPP

namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a C++ type.
    //!
    //! A `Type` is a special kind of object representing a C++ type like
    //! `int`, `void`, `std::vector<float>` or anything else you can imagine.
    //!
    //! This page explains how `Type`s work at a low level. To gain intuition
    //! about type-level metaprogramming in Hana, you should read the
    //! [tutorial section](@ref tutorial-type) on type-level computations.
    //!
    //!
    //! The actual representation of a Type
    //! -----------------------------------
    //! For subtle reasons having to do with ADL, the actual type of the
    //! `type<T>` expression is not `_type<T>`. It is a dependent type
    //! which inherits `_type<T>`. Hence, you should never rely on the
    //! fact that `type<T>` is of type `_type<T>`, but you can rely on
    //! the fact that it inherits it, which is different in some contexts,
    //! e.g. for template specialization.
    //!
    //!
    //! Lvalues and rvalues
    //! -------------------
    //! When storing `Type`s in heterogeneous containers, some algorithms will
    //! return references to those objects. Since we are primarily interested
    //! in accessing their nested `::type`, receiving a reference is
    //! undesirable; we would end up trying to fetch the nested `::type`
    //! inside a reference type, which is a compilation error:
    //! @code
    //!   auto ts = make_tuple(type<int>, type<char>);
    //!   using T = decltype(ts[0_c])::type; // error: 'ts[0_c]' is a reference!
    //! @endcode
    //!
    //! For this reason, `Type`s provide an overload of the unary `+` operator
    //! that can be used to turn a lvalue into a rvalue. So when using a result
    //! which might be a reference to a `Type` object, one can use `+` to make
    //! sure a rvalue is obtained before fetching its nested `::type`:
    //! @code
    //!   auto ts = make_tuple(type<int>, type<char>);
    //!   using T = decltype(+ts[0_c])::type; // ok: '+ts[0_c]' is an rvalue
    //! @endcode
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two `Type`s are equal if and only if they represent the same C++ type.
    //! Hence, equality is equivalent to the `std::is_same` type trait.
    //! @snippet example/type.cpp comparable
    struct Type { };

    //! Creates an object representing the C++ type `T`.
    //! @relates Type
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T>
    constexpr unspecified-type type{};
#else
    template <typename T>
    struct _type {
        struct _;
    };

    template <typename T>
    constexpr typename _type<T>::_ type{};
#endif

    namespace gcc_wknd {
        template <typename T>
        constexpr auto mktype() { return type<T>; }
    }

    //! `decltype` keyword, lifted to Hana.
    //! @relates Type
    //!
    //! `decltype_` is somewhat equivalent to `decltype` in that it returns
    //! the type of an object, except it returns it as a `Type` object which
    //! is a first-class citizen of Hana instead of a raw C++ type.
    //! Specifically, given an object `x`, `decltype_` satisfies
    //! @code
    //!   decltype_(x) == type<decltype(x) with references stripped>
    //! @endcode
    //!
    //! As you can see, `decltype_` will strip any reference from the
    //! object's actual type. The reason for doing so is explained below.
    //! However, any `cv`-qualifiers will be retained. Also, when given a
    //! `Type` object, `decltype_` is just the identity function. Hence, for
    //! any C++ type `T`,
    //! @code
    //!   decltype_(type<T>) == type<T>
    //! @endcode
    //!
    //! In conjunction with the way `metafunction` & al. are specified, this
    //! behavior makes it easier to interact with both types and values at
    //! the same time. However, it does make it impossible to create a Type
    //! containing a Type with `decltype_`. In other words, it is not possible
    //! to create a `type<decltype(type<T>)>` with this utility, because
    //! `decltype_(type<T>)` would be just `type<T>` instead of
    //! `type<decltype(type<T>)>`. This use case is assumed to be
    //! rare and a hand-coded function can be used if this is needed.
    //!
    //!
    //! ### Rationale for stripping the references
    //! The rules for template argument deduction are such that a perfect
    //! solution that always matches `decltype` is impossible. Hence, we
    //! have to settle on a solution that's good and and consistent enough
    //! for our needs. One case where matching `decltype`'s behavior is
    //! impossible is when the argument is a plain, unparenthesized variable
    //! or function parameter. In that case, `decltype_`'s argument will be
    //! deduced as a reference to that variable, but `decltype` would have
    //! given us the actual type of that variable, without references. Also,
    //! given the current definition of `metafunction` & al., it would be
    //! mostly useless if `decltype_` could return a reference, because it
    //! is unlikely that `F` expects a reference in its simplest use case:
    //! @code
    //!   int i = 0;
    //!   auto result = metafunction<F>(i);
    //! @endcode
    //!
    //! Hence, always discarding references seems to be the least painful
    //! solution.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp decltype_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto decltype_ = see documentation;
#else
    struct _decltype {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr _decltype decltype_{};
#endif

#ifdef BOOST_HANA_DOXYGEN_INVOKED
    //! Equivalent to `decltype_`, provided for convenience.
    //! @relates Type
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp make<Type>
    template <>
    constexpr auto make<Type> = decltype_;
#endif

    //! `sizeof` keyword, lifted to Hana.
    //! @relates Type
    //!
    //! `sizeof_` is somewhat equivalent to `sizeof` in that it returns the
    //! size of an expression or type, but it takes an arbitrary expression
    //! or a Type object and returns its size as an IntegralConstant.
    //! Specifically, given an expression `expr`, `sizeof_` satisfies
    //! @code
    //!   sizeof_(expr) == size_t<sizeof(decltype(expr) with references stripped)>
    //! @endcode
    //!
    //! However, given a Type object, `sizeof_` will simply fetch the size
    //! of the C++ type represented by that object. In other words,
    //! @code
    //!   sizeof_(type<T>) == size_t<sizeof(T)>
    //! @endcode
    //!
    //! The behavior of `sizeof_` is consistent with that of `decltype_`.
    //! In particular, see `decltype_`'s documentation to understand why
    //! references are always stripped by `sizeof_`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp sizeof_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sizeof_ = [](auto&& x) {
        using T = typename decltype(decltype_(x))::type;
        return size_t<sizeof(T)>;
    };
#else
    struct _sizeof {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr _sizeof sizeof_{};
#endif

    //! `alignof` keyword, lifted to Hana.
    //! @relates Type
    //!
    //! `alignof_` is somewhat equivalent to `alignof` in that it returns the
    //! alignment required by any instance of a type, but it takes a Type
    //! object and returns its alignment as an IntegralConstant. Like `sizeof`
    //! which works for expressions and type-ids, `alignof_` can also be
    //! called on an arbitrary expression. Specifically, given an expression
    //! `expr` and a C++ type `T`, `alignof_` satisfies
    //! @code
    //!   alignof_(expr) == size_t<alignof(decltype(expr) with references stripped)>
    //!   alignof_(type<T>) == size_t<alignof(T)>
    //! @endcode
    //!
    //! The behavior of `alignof_` is consistent with that of `decltype_`.
    //! In particular, see `decltype_`'s documentation to understand why
    //! references are always stripped by `alignof_`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp alignof_
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto alignof_ = [](auto&& x) {
        using T = typename decltype(decltype_(x))::type;
        return size_t<alignof(T)>;
    };
#else
    struct _alignof {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr _alignof alignof_{};
#endif

    //! Checks whether a SFINAE-friendly expression is valid.
    //! @relates Type
    //!
    //! Given a SFINAE-friendly function, `is_valid` returns whether the
    //! function call is valid with the given arguments. Specifically, given
    //! a function `f` and arguments `args...`,
    //! @code
    //!   is_valid(f, args...) == whether f(args...) is valid
    //! @endcode
    //!
    //! The result is returned as a compile-time `Logical`. Furthermore,
    //! `is_valid` can be used in curried form as follows:
    //! @code
    //!   is_valid(f)(args...)
    //! @endcode
    //!
    //! This syntax makes it easy to create functions that check the validity
    //! of a generic expression on any given argument(s).
    //!
    //! @warning
    //! To check whether calling a nullary function `f` is valid, one should
    //! use the `is_valid(f)()` syntax. Indeed, `is_valid(f /* no args */)`
    //! will be interpreted as the currying of `is_valid` to `f` rather than
    //! the application of `is_valid` to `f` and no arguments.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp is_valid
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_valid = [](auto&& f) {
        return [](auto&& ...args) {
            return whether f(args...) is a valid expression;
        };
    };
#else
    struct _is_valid {
        template <typename F>
        constexpr auto operator()(F&&) const;

        template <typename F, typename ...Args>
        constexpr auto operator()(F&&, Args&&...) const;
    };

    constexpr _is_valid is_valid{};
#endif

    //! @ingroup group-datatypes
    //! A `Metafunction` is a function that takes `Type`s as inputs and
    //! gives a `Type` as output.
    //!
    //! Hana provides adapters for representing several different kinds of
    //! metafunctions in a unified way, as normal C++ functions. The general
    //! process is always essentially the same. For example, given a MPL
    //! metafunction, we can create a normal C++ function that takes `Type`
    //! objects and returns a `Type` object containing the result of that
    //! metafunction:
    //! @code
    //!     template <template <typename ...> class F>
    //!     struct _metafunction {
    //!         template <typename ...T>
    //!         constexpr auto operator()(_type<T> const& ...) const {
    //!             return hana::type<typename F<T...>::type>;
    //!         }
    //!     };
    //!
    //!     template <template <typename ...> class F>
    //!     constexpr _metafunction<F> metafunction{};
    //! @endcode
    //!
    //! The variable template is just for convenience, so we can write
    //! `metafunction<F>` instead of `_metafunction<F>{}`, but they are
    //! otherwise equivalent. With the above construct, it becomes possible
    //! to perform type computations with the usual function call syntax, by
    //! simply wrapping our classic metafunctions into a `metafunction` object:
    //! @code
    //!     constexpr auto add_pointer = metafunction<std::add_pointer>;
    //!     static_assert(add_pointer(type<int>) == type<int*>, "");
    //! @endcode
    //!
    //! While this covers only classical MPL-style metafunctions, Hana
    //! provides wrappers for several constructs that are commonly used
    //! to embed type computations: template aliases and MPL-style
    //! metafunction classes. A type computation wrapped into such a
    //! construct is what Hana calls a `Metafunction`. However, note that
    //! not all metafunctions can be lifted this way. For example,
    //! `std::aligned_storage` can't be lifted because it requires non-type
    //! template parameters. Since there is no uniform way of dealing with
    //! non-type template parameters, one must resort to using e.g. an
    //! inline lambda to "lift" those metafunctions. In practice, however,
    //! this should not be a problem.
    //!
    //! __Example of a non-liftable metafunction__
    //! @snippet example/type.cpp non_liftable_metafunction
    //!
    //!
    //! In addition to being Callable, `Metafunction`s provide a nested `apply`
    //! template which allows performing the same type-level computation as
    //! is done by the call operator. In Boost.MPL parlance, a `Metafunction`
    //! `F` is also a Boost.MPL MetafunctionClass in addition to being a
    //! function on `Type`s. In other words again, a `Metafunction` `f` will
    //! satisfy:
    //! @code
    //!     f(type<T>...) == type<decltype(f)::apply<T...>::type>
    //! @endcode
    //!
    //! But that is not all. To ease the inter-operation of values and types,
    //! `Metafunction`s also allow being called with arguments that are not
    //! `Type`s. In that case, the result is equivalent to calling the
    //! metafunction on the result of `decltype_`ing the arguments.
    //! Specifically, given a `Metafunction` `f` and arbitrary (`Type` or
    //! non-`Type`) objects `x...`,
    //! @code
    //!     f(x...) == f(decltype_(x)...)
    //! @endcode
    //!
    //! So `f` is called with the type of its arguments, but since `decltype_`
    //! is just the identity for `Type`s, only non-`Type`s are lifted to the
    //! `Type` level.
    struct Metafunction { };

    //! Lift a template to a Metafunction.
    //! @relates Metafunction
    //!
    //! Given a template class or template alias `f`, `template_<f>` is a
    //! `Metafunction` satisfying
    //! @code
    //!     template_<f>(type<x>...) == type<f<x...>>
    //!     decltype(template_<f>)::apply<x...>::type == f<x...>
    //! @endcode
    //!
    //! @note
    //! `template_` can't be SFINAE-friendly right now because of
    //! [Core issue 1430][1].
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp template
    //!
    //! [1]: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class F>
    constexpr auto template_ = [](_type<T>-or-T ...) {
        return type<F<T...>>;
    };
#else
    template <template <typename ...> class F>
    struct _template;

    template <template <typename ...> class F>
    constexpr _template<F> template_{};
#endif

    //! Lift a MPL-style metafunction to a function on `Type`s.
    //! @relates Metafunction
    //!
    //! Given a MPL-style metafunction, `metafunction<f>` is a `Metafunction`
    //! satisfying
    //! @code
    //!     metafunction<f>(type<x>...) == type<f<x...>::type>
    //!     decltype(metafunction<f>)::apply<x...>::type == f<x...>::type
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp metafunction
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class F>
    constexpr auto metafunction = [](_type<T>-or-T ...) {
        return type<typename F<T...>::type>;
    };
#else
    template <template <typename ...> class f>
    struct _metafunction;

    template <template <typename ...> class f>
    constexpr _metafunction<f> metafunction{};
#endif

    //! Lift a MPL-style metafunction class to a function on `Type`s.
    //! @relates Metafunction
    //!
    //! Given a MPL-style metafunction class, `metafunction_class<f>` is a
    //! `Metafunction` satisfying
    //! @code
    //!     metafunction_class<f>(type<x>...) == type<f::apply<x...>::type>
    //!     decltype(metafunction_class<f>)::apply<x...>::type == f::apply<x...>::type
    //! @endcode
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp metafunction_class
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename F>
    constexpr auto metafunction_class = [](_type<T>-or-T ...) {
        return type<typename F::template apply<T...>::type>;
    };
#else
    template <typename F>
    struct _metafunction_class
        : _metafunction<F::template apply>
    { };

    template <typename F>
    constexpr _metafunction_class<F> metafunction_class{};
#endif

    //! Turn a `Metafunction` into a function taking `Type`s and returning a
    //! default-constructed object.
    //! @relates Metafunction
    //!
    //! Given a `Metafunction` `f`, `integral` returns a new `Metafunction`
    //! that default-constructs an object of the type returned by `f`. More
    //! specifically, the following holds:
    //! @code
    //!     integral(f)(t...) == decltype(f(t...))::type{}
    //! @endcode
    //!
    //! The principal use case for `integral` is to transform `Metafunction`s
    //! returning a type that inherits from a meaningful base like
    //! `std::integral_constant` into functions returning e.g. an
    //! `IntegralConstant`.
    //!
    //! @note
    //! - This is not a `Metafunction` because it does not return a `Type`.
    //!   As such, it would not make sense to make `decltype(integral(f))`
    //!   a MPL metafunction class like the usual `Metafunction`s are.
    //!
    //! - When using `integral` with metafunctions returning
    //!   `std::integral_constant`s, don't forget to include the
    //!   boost/hana/ext/std/integral_constant.hpp header to ensure
    //!   Hana can interoperate with the result.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp integral
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto integral = [](auto f) {
        return [](_type<T>-or-T ...) {
            return decltype(f)::apply<T...>::type{};
        };
    };
#else
    template <typename F>
    struct _integral;

    struct _make_integral {
        template <typename F>
        constexpr _integral<F> operator()(F const&) const
        { return {}; }
    };

    constexpr _make_integral integral{};
#endif

    //! Alias to `integral(metafunction<F>)`, provided for convenience.
    //! @relates Metafunction
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/type.cpp trait
    template <template <typename ...> class F>
    constexpr auto trait = integral(metafunction<F>);
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TYPE_HPP
