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


namespace boost { namespace hana {
    //! @datatype{Type}
    //! Wrapper to manipulate C++ types as constexpr objects.
    //!
    //! @todo
    //! - Completely figure out and document the category theoretical
    //!   foundation of this data type.
    //! - Verify `Monad` laws.
    //! - Move self-notes for `Type`-related stuff to the (internal?)
    //!   documentation of `Type`.
    //! - Document instances.
    //! - Consider having a `.name()` method that would return the
    //!   (demangled?) `typeid(T).name()`.
    //! - Add examples.
    struct Type { };

    //! Creates a `Type` representing `T`.
    //! @relates Type
    template <typename T>
    BOOST_HANA_CONSTEXPR_LAMBDA auto type = [] {
        struct wrapper : operators::enable {
            using hana_datatype = Type;
            using hidden = T;
        };
        return wrapper{};
    }();

    //! Metafunction returning the type represented by a `Type`.
    //! @relates Type
    //!
    //! @note This operation is the inverse of `type`.
    //!
    //! @todo Explain why this must be a metafunction, with examples.
    template <typename T>
    struct untype {
        using type = typename T::hidden;
    };

    //! Alias to `untype<T>::type`; provided for convenience.
    //! @relates Type
    template <typename T>
    using untype_t = typename untype<T>::type;

    //! Returns the type of an object as a `Type`.
    //! @relates Type
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
     */
    template <template <typename ...> class f>
    constexpr type_detail::Template<f> template_{};

    template <>
    struct Comparable<Type, Type> : defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(T, U)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(T, T)
        { return true_; }
    };


    template <>
    struct Functor<Type> : defaults<Functor> {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, T t)
        { return f(t); }
    };

    template <>
    struct Monad<Type> : defaults<Monad> {
        template <typename T>
        static constexpr auto unit_impl(T t)
        { return decltype_(t); }

        template <typename T>
        static constexpr auto join_impl(T)
        { return untype_t<T>{}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
