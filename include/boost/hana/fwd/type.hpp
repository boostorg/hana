/*!
@file
Forward declares `boost::hana::Type`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TYPE_HPP
#define BOOST_HANA_FWD_TYPE_HPP

#include <boost/hana/core/operators.hpp>
#include <boost/hana/fwd/comparable.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Object representing a C++ type.
    //!
    //! `Type`s are the key element making it possible to do everything that
    //! can be done with Boost.MPL also doable with Hana. A `Type` is an
    //! object of an unspecified type which represents (or wraps) a C++ type.
    //! However, since it is a regular C++ object, it can be stored in
    //! heterogeneous containers, passed as an argument to regular functions
    //! and so on. While representing C++ types as values might seem backward
    //! at first, it is in fact the very thing that makes it possible to unify
    //! type-level programming and value-level programming. In particular,
    //! this allows using the same syntax for type-level computations as used
    //! in everyday C++, and it even allows certain functions with very few
    //! requirements to be used both on types and on values, thus reducing
    //! code duplication. In comparison, previous metaprogramming libraries
    //! like [Boost.MPL][] or [MPL11][] used struct-based metafunctions that
    //! worked at the type level only, were less expressive due to the lack
    //! of anonymous functions and forced one to use an awkward syntax.
    //!
    //! Of course, since each object of data type `Type` has an unspecified
    //! (and hence possibly different) C++ type, manipulating these guys
    //! requires being able to manipulate heterogeneous objects. Fortunately,
    //! this is exactly what this library was built for, so we're good.
    //!
    //! By wrapping C++ types into `Type` objects, we're only able to pass
    //! wrapped types around, which isn't very useful. For `Type`s to be of
    //! some use, they have to give us a way of getting that wrapped type out.
    //! Hence, for any `Type` `t`, `decltype(t)::type` is an alias to the type
    //! it wraps. In Boost.MPL parlance, `decltype(t)` is a nullary metafunction
    //! returning the C++ type `t` represents:
    //!
    //! @snippet example/type/as_metafunction.cpp main
    //!
    //!
    //! ### Lvalues and rvalues
    //!
    //! When storing `Type`s in heterogeneous containers, some algorithms will
    //! return references to those objects. Since we are primarily interested
    //! in accessing the `::type` inside of these objects, receiving a
    //! reference is undesirable; we would end up trying to fetch the nested
    //! `::type` inside a reference type, which is a compilation error:
    //! @code
    //!     // Error; decltype(...) is a reference!
    //!     using T = decltype(at_c<1>(tuple(type<int>, type<char>)))::type;
    //! @endcode
    //!
    //! For this reason, `Type`s provide an overload of the unary `+` operator
    //! that can be used to turn a lvalue into a rvalue. So when using a result
    //! which might be a reference to a `Type` object, one can use `+` to make
    //! sure a rvalue is obtained before fetching its nested `::type`:
    //! @code
    //!     // Good; decltype(+...) is an rvalue.
    //!     using T = decltype(+at_c<1>(tuple(type<int>, type<char>)))::type;
    //! @endcode
    //!
    //!
    //! ### Instance of
    //! `Comparable`
    //!
    //!
    //! @todo
    //! - Completely figure out and document the category theoretical
    //!   foundation of this data type.
    //! - Consider instantiating `Functor`, `Applicative` and `Monad` if
    //!   that's possible.
    //! - Consider having a `.name()` method that would return the
    //!   (demangled?) `typeid(T).name()`.
    //!
    //! @bug
    //! `metafunction` and friends are not SFINAE-friendly right now. See
    //! [this GCC bug][GCC_58498] and also [Core 1430 issue][Core_1430] issue.
    //! Once this issue is resolved, look at the unit tests for those utilities
    //! and either uncomment or remove the relevant test section.
    //!
    //!
    //! [Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
    //! [MPL11]: http://github.com/ldionne/mpl11
    //! [Core_1430]: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
    //! [GCC_58498]: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=59498
    struct Type {
        struct hana {
            struct enabled_operators
                : Comparable
            { };
        };
    };

    struct Metafunction { };

    template <typename T>
    struct _type {
        struct _ : operators::enable_adl {
            struct hana { using datatype = Type; };
            using type = T;

            constexpr _ operator+() const { return *this; }
        };
    };

    //! Creates an object representing the C++ type `T`.
    //! @relates Type
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T>
    constexpr unspecified-type type{};
#else
    template <typename T>
    constexpr typename _type<T>::_ type{};
#endif

    //! Returns the type of an object as a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/decltype.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto decltype_ = [](auto x) {
        return type<decltype(x)>;
    };
#else
    struct _decltype {
        template <typename T>
        constexpr auto operator()(T) const
        { return type<T>; }
    };

    constexpr _decltype decltype_{};
#endif

    //! Returns the size of the C++ type represented by a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/sizeof.cpp main
    //!
    //! @todo
    //! Should we also support non-`Type`s? That could definitely be useful.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sizeof_ = [](auto t) {
        using T = typename decltype(t)::type;
        return size_t<sizeof(T)>;
    };
#else
    struct _sizeof {
        template <typename T>
        constexpr auto operator()(T) const;
    };

    constexpr _sizeof sizeof_{};
#endif

    //! Lift a template to a function on `Type`s.
    //! @relates Type
    //!
    //! Specifically, `template_<f>` is a function on `Type`s satisfying
    //! @code
    //!     template_<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>>
    //! @endcode
    //!
    //! `decltype(template_<f>)` is also a metafunction class such that
    //! @code
    //!     decltype(template_<f>)::apply<x1, ..., xN>::type == f<x1, ..., xN>
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/type/template.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class f>
    constexpr auto template_ = [](auto ...ts) {
        return type<
            f<typename decltype(ts)::type...>
        >;
    };
#else
    template <template <typename ...> class f>
    struct _template {
        struct hana { using datatype = Metafunction; };

        template <typename ...xs>
        struct apply {
            using type = f<xs...>;
        };

        template <typename ...xs>
        constexpr auto operator()(xs...) const
        { return type<f<typename xs::type...>>; }
    };

    template <template <typename ...> class f>
    constexpr _template<f> template_{};
#endif

    //! Lift a metafunction to a function on `Type`s.
    //! @relates Type
    //!
    //! Specifically, `metafunction<f>` is a function on `Type`s satisfying
    //! @code
    //!     metafunction<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>::type>
    //! @endcode
    //!
    //! `decltype(metafunction<f>)` is also a metafunction class such that
    //! @code
    //!     decltype(metafunction<f>)::apply<x1, ..., xN> == f<x1, ..., xN>
    //! @endcode
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class f>
    constexpr auto metafunction = [](auto ...ts) {
        return type<
            typename f<typename decltype(ts)::type...>::type
        >;
    };
#else
    template <template <typename ...> class f>
    struct _metafunction {
        struct hana { using datatype = Metafunction; };

        template <typename ...xs>
        using apply = f<xs...>;

        template <typename ...xs>
        constexpr auto operator()(xs...) const
        { return type<typename f<typename xs::type...>::type>; }
    };

    template <template <typename ...> class f>
    constexpr _metafunction<f> metafunction{};
#endif

    //! Lift a metafunction class to a function on `Type`s.
    //! @relates Type
    //!
    //! Specifically, `metafunction_class<f>` is a function on `Type`s
    //! satisfying
    //! @code
    //!     metafunction_class<f>(type<x1>, ..., type<xN>) == type<f::apply<x1, ..., xN>::type>
    //! @endcode
    //!
    //! `decltype(metafunction_class<f>)` is also a metafunction class such that
    //! @code
    //!     decltype(metafunction_class<f>)::apply<x1, ..., xN> == f::apply<x1, ..., xN>
    //! @endcode
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename f>
    constexpr auto metafunction_class = [](auto ...ts) {
        return type<
            typename f::template apply<
                typename decltype(ts)::type...
            >::type
        >;
    };
#else
    template <typename f>
    struct _metafunction_class {
        struct hana { using datatype = Metafunction; };

        template <typename ...xs>
        using apply = typename f::template apply<xs...>;

        template <typename ...xs>
        constexpr auto operator()(xs...) const {
            return type<
                typename f::template apply<typename xs::type...>::type
            >;
        }
    };

    template <typename f>
    constexpr _metafunction_class<f> metafunction_class{};
#endif

    //! Lift a metafunction to a function taking `Type`s and returning a
    //! default-constructed object.
    //! @relates Type
    //!
    //! Specifically, `trait<f>(t...)` is equivalent to `template_<f>(t...)()`.
    //! The principal use case for `trait` is to transform metafunctions
    //! inheriting from a meaningful base like `std::integral_constant`
    //! into functions returning e.g. an `Integral`.
    //!
    //! The word `trait` is used because a name was needed and the principal
    //! use case involves metafunctions from the standard that we also call
    //! type traits.
    //!
    //! ### Example
    //! @snippet example/type/trait.cpp liftable
    //!
    //! Note that not all metafunctions of the standard library can be lifted
    //! this way. For example, `std::aligned_storage` can't be lifted because
    //! it requires non-type template parameters. Since there is no uniform
    //! way of dealing with non-type template parameters, one must resort to
    //! using e.g. an inline lambda to "lift" those metafunctions. In practice,
    //! however, this should not be a problem.
    //!
    //! ### Example of a non-liftable metafunction
    //! @snippet example/type/trait.cpp nonliftable
    //!
    //! @note
    //! When using `trait` with metafunctions returning `std::integral_constant`s,
    //! don't forget to include the boost/hana/ext/std/integral_constant.hpp
    //! header!
    //!
    //! @note
    //! Since `trait<f>` does not return a `Type`, it does not really make
    //! sense to make `decltype(trait<f>)` a metafunction class, which
    //! explains the omission.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class f>
    constexpr auto trait = [](auto ...ts) {
        return f<typename decltype(ts)::type...>{};
    };
#else
    template <template <typename ...> class f>
    struct _trait {
        template <typename ...xs>
        constexpr auto operator()(xs...) const
        { return f<typename xs::type...>{}; }
    };

    template <template <typename ...> class f>
    constexpr _trait<f> trait{};
#endif

    //! Equivalent to `compose(trait<f>, decltype_)`; provided for convenience.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/trait_.cpp main
    //!
    //! @note
    //! Since `trait_<f>` does not return a `Type`, it does not really make
    //! sense to make `decltype(trait_<f>)` a metafunction class, which
    //! explains the omission.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <template <typename ...> class f>
    constexpr auto trait_ = [](auto ...xs) {
        return f<decltype(xs)...>{};
    };
#else
    template <template <typename ...> class f>
    struct _trait_ {
        template <typename ...xs>
        constexpr auto operator()(xs...) const
        { return f<xs...>{}; }
    };

    template <template <typename ...> class f>
    constexpr _trait_<f> trait_{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TYPE_HPP
