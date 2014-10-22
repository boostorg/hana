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
    //! of anonymous functions and forced the programmer to use an awkward
    //! syntax.
    //!
    //! Of course, since each object of data type `Type` has an unspecified
    //! (and hence possibly different) C++ type, manipulating these guys
    //! requires being able to manipulate heterogeneous objects. Fortunately,
    //! this is exactly what this library was built for, so we're good.
    //!
    //! So far, we're only able to pass wrapped types around, which isn't
    //! very useful. For `Type`s to be of some use, they have to give us
    //! a way of getting that wrapped type out. Hence, for any `Type` `t`,
    //! `decltype(t)::type` is an alias to the type it wraps. In Boost.MPL
    //! parlance, `decltype(t)` is a nullary metafunction returning the C++
    //! type `t` represents.
    //!
    //! [Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
    //! [MPL11]: http://github.com/ldionne/mpl11
    //!
    //! ## Instance of
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
    //! `metafunction` and friends are not SFINAE-friendly right now.
    //! See [this](http://gcc.gnu.org/bugzilla/show_bug.cgi?id=59498) GCC bug
    //! and also [Core 1430](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430)
    //! issue. Once this issue is resolved, look at the unit tests for those
    //! utilities and either uncomment or remove the relevant test section.
    struct Type { struct hana_enabled_operators : Comparable { }; };

    struct Metafunction { };

    namespace type_detail {
        template <typename T>
        struct make_type {
            struct hidden : operators::enable_adl {
                using hana_datatype = Type;
                using type = T;
            };
        };
    }

    //! Creates an object representing the C++ type `T`.
    //! @relates Type
    //! @hideinitializer
    //!
    //! `type<T>` is an object of an unspecified type such that
    //! `decltype(type<T>)` has a nested alias to `T` named `type`.
    //! In other words `decltype(type<T>)` is a Boost.MPL nullary
    //! metafunction returning `T`:
    //! @snippet example/type/as_metafunction.cpp main
    template <typename T>
    constexpr typename type_detail::make_type<T>::hidden type{};

    namespace type_detail {
        struct decltype_ {
            template <typename T>
            constexpr auto operator()(T) const
            { return type<T>; }
        };

        struct sizeof_ {
            template <typename T>
            constexpr auto operator()(T) const;
        };
    }

    //! Returns the type of an object as a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/decltype.cpp main
    constexpr type_detail::decltype_ decltype_{};

    //! Returns the size of the C++ type represented by a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/sizeof.cpp main
    //!
    //! @todo
    //! Should we also support non-`Type`s? That could definitely be useful.
    constexpr type_detail::sizeof_ sizeof_{};

    namespace type_detail {
        template <template <typename ...> class f>
        struct template_ {
            using hana_datatype = Metafunction;

            template <typename ...xs>
            struct apply {
                using type = f<xs...>;
            };

            template <typename ...xs>
            constexpr auto operator()(xs...) const
            { return type<f<typename xs::type...>>; }
        };

        template <template <typename ...> class f>
        struct metafunction {
            using hana_datatype = Metafunction;

            template <typename ...xs>
            using apply = f<xs...>;

            template <typename ...xs>
            constexpr auto operator()(xs...) const
            { return type<typename f<typename xs::type...>::type>; }
        };

        template <typename f>
        struct metafunction_class {
            using hana_datatype = Metafunction;

            template <typename ...xs>
            using apply = typename f::template apply<xs...>;

            template <typename ...xs>
            constexpr auto operator()(xs...) const
            { return type<typename f::template apply<typename xs::type...>::type>; }
        };

        template <template <typename ...> class f>
        struct trait {
            template <typename ...xs>
            constexpr auto operator()(xs...) const
            { return f<typename xs::type...>{}; }
        };

        template <template <typename ...> class f>
        struct trait_ {
            template <typename ...xs>
            constexpr auto operator()(xs...) const
            { return f<xs...>{}; }
        };
    }

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
    template <template <typename ...> class f>
    constexpr type_detail::template_<f> template_{};

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
    template <template <typename ...> class f>
    constexpr type_detail::metafunction<f> metafunction{};

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
    template <typename f>
    constexpr type_detail::metafunction_class<f> metafunction_class{};

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
    template <template <typename ...> class f>
    constexpr type_detail::trait<f> trait{};

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
    template <template <typename ...> class f>
    constexpr type_detail::trait_<f> trait_{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_TYPE_HPP
