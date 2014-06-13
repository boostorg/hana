/*!
@file
Defines `boost::hana::Type`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/monad.hpp>

#include <initializer_list>


namespace boost { namespace hana {
    /*!
    @ingroup datatypes
    C++ type represented as a constexpr object.

    --------------------------------------------------------------------------

    ## Instance of

    ### Comparable
    Two `Type`s are equal if and only if they represent the same C++ type.
    Hence, equality is equivalent to the `std::is_same` type trait.
    @snippet example/type/comparable.cpp main

    ### Functor
    TODO

    ### Monad
    TODO

    --------------------------------------------------------------------------

    @todo
    - Completely figure out and document the category theoretical foundation
      of this data type.
    - Verify `Monad` laws.
    - Move self-notes for `Type`-related stuff to the (internal?)
      documentation of `Type`.
    - Consider having a `.name()` method that would return the
      (demangled?) `typeid(T).name()`.
    - Document `Functor` and `Monad` instances.
     */
    struct Type { };

    namespace type_detail {
        template <typename Wrapper>
        struct construct {
            template <typename T>
            constexpr auto operator()(std::initializer_list<T> ilist) const
            { return typename Wrapper::hidden(ilist); }

            template <typename ...Args>
            constexpr auto operator()(Args ...args) const
            { return typename Wrapper::hidden{args...}; }
        };
    }

    /*!
    Creates a `Type` representing `T`.
    @relates Type
    @hideinitializer

    Additionally, `type<T>` is a function returning an object of type
    `T` constructed with the arguments passed to it.

    ### Example
    @snippet example/type/construct.cpp main

    @note
    `std::initializer_list` is supported too:
    @snippet example/type/initializer_list.cpp main

    @todo
    Should this fail or not? Currently it fails because
    "non-constant-expression cannot be narrowed from type 'double' to
    'float' in initializer list"
    @code
        type<float>(double{1.2})
    @endcode

    @todo
    Consider making `type<>` equivalent to `decltype_`, and then removing
    `decltype_`.\n
    Pros:
    - Reduces the number of names we have to remember. Right now, it's
      really messy with `decltype_`, `type<T>`, `untype` and all that fluff.
    - `type<>` has 3 letters less than `decltype_`. Not a big pro,
      but still.\n
    Cons:
    - Too much overloading of the same name with different semantics can
      yield the opposite effect and be messier than using different names.
     */
    template <typename T>
    BOOST_HANA_CONSTEXPR_LAMBDA auto type = [] {
        struct wrapper : operators::enable, type_detail::construct<wrapper> {
            using hana_datatype = Type;
            using hidden = T;
        };
        return wrapper{};
    }();

#if defined(BOOST_HANA_DOXYGEN_INVOKED)
    //! Metafunction returning the type represented by a `Type`.
    //! @relates Type
    //!
    //! This operation is the inverse of `type`. It must be a metafunction
    //! because if it were a function, it would have to return an object,
    //! which is not always possible (consider uninhabited types).
    //!
    //! ### Example
    //! @snippet example/type/untype.cpp main
    //!
    //! @internal
    //! internal: Keep this an alias so it stays grouped with `Type` in
    //! the documentation.
    template <typename T>
    using untype = unspecified;
#else
    template <typename T>
    struct untype {
        using type = typename T::hidden;
    };
#endif

    //! @relates Type
    template <typename T>
    using untype_t = typename untype<T>::type;

    //! Returns the type of an object as a `Type`.
    //! @relates Type
    //!
    //! ### Example
    //! @snippet example/type/decltype.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto decltype_ = [](auto t) {
        return type<decltype(t)>;
    };

    namespace type_detail {
        template <template <typename ...> class f>
        struct Template {
            template <typename ...Args>
            constexpr auto operator()(Args...) const
            { return type<f<untype_t<Args>...>>; }
        };
    }

    /*!
    Wraps a template as a constexpr object.
    @relates Type

    Additionally, `template_<f>` is a function on `Type`s satisfying
    @code
        template_<f>(type<x1>, ..., type<xN>) == type<f<x1, ..., xN>>
    @endcode

    ### Example
    @snippet example/type/template.cpp main
     */
    template <template <typename ...> class f>
    constexpr type_detail::Template<f> template_{};

    template <>
    struct Comparable<Type, Type>
        : defaults<Comparable>::template with<Type, Type>
    {
        template <typename T, typename U>
        static constexpr auto equal_impl(T, U)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(T, T)
        { return true_; }
    };


    template <>
    struct Functor<Type> : defaults<Functor>::with<Type> {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, T t)
        { return f(t); }
    };

    template <>
    struct Monad<Type> : defaults<Monad>::with<Type> {
        template <typename T>
        static constexpr auto unit_impl(T t)
        { return decltype_(t); }

        template <typename T>
        static constexpr auto join_impl(T)
        { return untype_t<T>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
