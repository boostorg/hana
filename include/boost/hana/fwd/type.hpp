/*!
@file
Forward declares `boost::hana::Type` and `boost::hana::Metafunction`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_TYPE_HPP
#define BOOST_HANA_FWD_TYPE_HPP

#include <boost/hana/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-datatypes
    //! Represents a C++ type.
    //!
    //! @note
    //! This page explains how Types work at a low level. To gain intuition
    //! about type-level metaprogramming in Hana, you should read the
    //! [tutorial section](@ref tutorial-type_computations) on type level
    //! computations.
    //!
    //! A `Type` is a special kind of object representing a C++ type like
    //! `int`, `void`, `std::vector<float>` or anything else you can imagine.
    //! Basically, the trick to implement such an object is to create the
    //! following dummy type:
    //! @code
    //!     template <typename T>
    //!     struct _type { };
    //! @endcode
    //!
    //! Now, if we want to represent the type `int` by an object, we just
    //! create the following object
    //! @code
    //!     _type<int> foo;
    //! @endcode
    //! and pretend that `foo` represents the type `int`. Note that since
    //! `_type<int>` can only be default constructed and hence has only one
    //! value, we could even not bother giving this object a name and we
    //! could simply use the `_type<int>{}` expression. The point here is
    //! that there is nothing special about the `foo` variable; it is just
    //! an alias for `_type<int>{}`.
    //!
    //! > __Note__
    //! > This is not exactly how `Type`s are implemented in Hana because of
    //! > some subtleties; things were dumbed down here for the sake of
    //! > clarity. Please check below to know exactly what you can expect
    //! > from a `Type`.
    //!
    //! Now, let's say we wanted to transform our type `int` (represented by
    //! `foo`) into a type `int*` (represented by some other variable); how
    //! could we do that? More generally, how could we transform a type `T`
    //! into a type `T*`? Let's write a function!
    //! @code
    //!     template <typename T>
    //!     _type<T*> add_pointer(_type<T> foo) {
    //!         _type<T*> bar;
    //!         return bar;
    //!     }
    //! @endcode
    //!
    //! We just let the compiler deduce the `T`, and from that we are able to
    //! generate the proper return type. That's it for the signature. For the
    //! implementation, we provide the simplest one that will make the code
    //! compile; we create a dummy object of the proper type and we return it.
    //! We can now use our function like:
    //! @code
    //!     auto bar = add_pointer(foo);
    //!     auto baz = add_pointer(bar);
    //! @endcode
    //! and we now have objects that represent the types `int*` and `int**`,
    //! respectively.
    //!
    //! As a side note, since we're lazy and we want to save as many
    //! keystrokes as possible, we'll use a variable template (new in
    //! C++14) to create our dummy variables:
    //! @code
    //!     template <typename T>
    //!     _type<T> type;
    //! @endcode
    //! Instead of typing `foo` or `_type<int>{}`, we can now simply write
    //! `type<int>`, which is effectively the same but looks better.
    //!
    //! However, the current definition of `_type` does not make it very
    //! useful. Indeed, we are only able to copy those objects around and
    //! perform pattern matching in template functions, which is still a bit
    //! limited. To make them more widely useful, we add the requirement
    //! that a `_type<T>` provides a nested alias to the type it wraps.
    //! In Boost.MPL parlance, we make `_type<T>` a nullary metafunction:
    //! @code
    //!     template <typename T>
    //!     struct _type {
    //!         using type = T;
    //!     };
    //! @endcode
    //!
    //! Now, we can get the type represented by one of our objects without
    //! having to perform pattern matching inside a template function:
    //! @code
    //!     auto bar = type<int*>;
    //!     using Bar = decltype(bar)::type;
    //!     static_assert(std::is_same<int*, Bar>{}, "");
    //! @endcode
    //!
    //! Also, this makes any function returning a `Type` easily usable as a
    //! classic metafunction, by simply using decltype. For example, let's
    //! consider the following function, which finds the largest type in
    //! a sequence of types:
    //!
    //! @snippet example/type.cpp largest
    //!
    //! To make it a classic metafunction instead, we only need to modify it
    //! slightly using `decltype`:
    //!
    //! @snippet example/type.cpp largest2
    //!
    //! While this new paradigm for type level programming might be difficult
    //! to grok at first, it will make more sense as you use it more and more.
    //! You will also come to appreciate how it blurs the line between types
    //! and values, opening new exciting possibilities.
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
    //!     auto ts = std::make_tuple(type<int>, type<char>);
    //!     // Error; decltype(...) is a reference!
    //!     using T = decltype(std::get<1>(ts))::type;
    //! @endcode
    //!
    //! For this reason, `Type`s provide an overload of the unary `+` operator
    //! that can be used to turn a lvalue into a rvalue. So when using a result
    //! which might be a reference to a `Type` object, one can use `+` to make
    //! sure a rvalue is obtained before fetching its nested `::type`:
    //! @code
    //!     auto ts = std::make_tuple(type<int>, type<char>);
    //!     // Good; decltype(+...) is an rvalue.
    //!     using T = decltype(+std::get<1>(ts))::type;
    //! @endcode
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
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two `Type`s are equal if and only if they represent the same C++ type.
    //! Hence, equality is equivalent to the `std::is_same` type trait.
    //! @snippet example/type.cpp comparable
    //!
    //!
    //! @todo
    //! - Completely figure out and document the category theoretical
    //!   foundation of this data type.
    //! - Consider instantiating `Functor`, `Applicative` and `Monad` if
    //!   that's possible.
    //!
    //! @bug
    //! `metafunction` and friends are not SFINAE-friendly right now. See
    //! [this GCC bug][GCC_58498] and also [Core 1430 issue][Core_1430] issue.
    //! Once this issue is resolved, look at the unit tests for those utilities
    //! and either uncomment or remove the relevant test section.
    //!
    //!
    //! [Core_1430]: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
    //! [GCC_58498]: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=59498
    struct Type { };

    //! Creates an object representing the C++ type `T`.
    //! @relates Type
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T>
    constexpr unspecified-type type{};
#else
    template <typename T>
    struct _type {
        struct _ : operators::adl {
            struct hana { using datatype = Type; };
            using type = T;

            constexpr _ operator+() const { return *this; }
        };
    };

    template <typename T>
    constexpr typename _type<T>::_ type{};
#endif

    //! Returns the type of an object as a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type.cpp decltype
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
    //! @snippet example/type.cpp sizeof
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


    //! @ingroup group-datatypes
    //! A `Metafunction` is a function that takes `Type`s as inputs and
    //! gives a `Type` as output.
    //!
    //! In addition to the usual requirement of being callable, a
    //! `Metafunction` must provide a nested `apply` template to
    //! perform the same type-level computation as is done by its
    //! call operator. In Boost.MPL parlance, a `Metafunction` `F`
    //! must be a Boost.MPL MetafunctionClass in addition to being
    //! a function on `Type`s. In other words again, any `Metafunction`
    //! `f` must satisfy:
    //! @code
    //!     f(type<T1>, ..., type<Tn>) == type<decltype(f)::apply<T1, ..., Tn>::type>
    //! @endcode
    struct Metafunction { };

    //! Lift a template to a function on `Type`s.
    //! @relates Metafunction
    //!
    //! Specifically, `template_<f>` is a `Metafunction` satisfying
    //! @code
    //!     template_<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>>
    //!     decltype(template_<f>)::apply<x1, ..., xN>::type == f<x1, ..., xN>
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/type.cpp template
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

    //! Lift a MPL-style metafunction to a function on `Type`s.
    //! @relates Metafunction
    //!
    //! Specifically, `metafunction<f>` is a `Metafunction` satisfying
    //! @code
    //!     metafunction<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>::type>
    //!     decltype(metafunction<f>)::apply<x1, ..., xN>::type == f<x1, ..., xN>::type
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/type.cpp metafunction
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

    //! Lift a MPL-style metafunction class to a function on `Type`s.
    //! @relates Metafunction
    //!
    //! Specifically, `metafunction_class<f>` is a `Metafunction` satisfying
    //! @code
    //!     metafunction_class<f>(type<x1>, ..., type<xN>) == type<f::apply<x1, ..., xN>::type>
    //!     decltype(metafunction_class<f>)::apply<x1, ..., xN>::type == f::apply<x1, ..., xN>::type
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

    //! Lift a MPL-style metafunction to a function taking `Type`s and
    //! returning a default-constructed object.
    //! @relates Metafunction
    //!
    //! Specifically, `trait<f>(t...)` is equivalent to `template_<f>(t...)()`.
    //! The principal use case for `trait` is to transform metafunctions
    //! inheriting from a meaningful base like `std::integral_constant`
    //! into functions returning e.g. an `IntegralConstant`.
    //!
    //! The word `trait` is used because a name was needed and the principal
    //! use case involves metafunctions from the standard that we also call
    //! type traits.
    //!
    //! @note
    //! This is not a `Metafunction` because it does not return a `Type`.
    //! In particular, it would not make sense to make `decltype(trait<f>)`
    //! a MPL metafunction class.
    //!
    //! ### Example
    //! @snippet example/type.cpp liftable_trait
    //!
    //! Note that not all metafunctions of the standard library can be lifted
    //! this way. For example, `std::aligned_storage` can't be lifted because
    //! it requires non-type template parameters. Since there is no uniform
    //! way of dealing with non-type template parameters, one must resort to
    //! using e.g. an inline lambda to "lift" those metafunctions. In practice,
    //! however, this should not be a problem.
    //!
    //! ### Example of a non-liftable metafunction
    //! @snippet example/type.cpp non_liftable_trait
    //!
    //! @note
    //! When using `trait` with metafunctions returning `std::integral_constant`s,
    //! don't forget to include the boost/hana/ext/std/integral_constant.hpp
    //! header!
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
    //! @relates Metafunction
    //!
    //! @note
    //! This is not a `Metafunction` because it does not return a `Type`.
    //! In particular, it would not make sense to make `decltype(trait_<f>)`
    //! a MPL metafunction class.
    //!
    //! ### Example
    //! @snippet example/type.cpp trait_
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
