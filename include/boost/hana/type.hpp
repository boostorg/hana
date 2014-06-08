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
    //! - Document the category theoretical foundation of the `Monad` instance.
    //! - Verify `Monad` laws.
    //! - Move self-notes for `Type`-related stuff to the (internal?)
    //!   documentation of `Type`.
    //! - Document instances.
    //! - Consider having a `.name()` method that would return the
    //!   (demangled?) `typeid(T).name()`.
    //! - Add examples.
    struct Type { };

    namespace operators {
        template <typename T>
        struct _type {
            using hana_datatype = Type;
            using type = T;
        };
    }

    //! Creates a `Type` representing `T`.
    //! @relates Type
    template <typename T>
    constexpr operators::_type<T> type{};

    namespace type_detail {
        template <template <typename ...> class f>
        struct Template {
            template <typename ...Args>
            constexpr auto operator()(operators::_type<Args>...) const
            { return type<f<Args...>>; }
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

    struct _untype {
        template <typename T>
        constexpr T operator()(operators::_type<T>) const
        { return {}; }
    };

    //! Default constructs an object of the type wrapped in a `Type`.
    //! @relates Type
    //!
    //! Obviously, this function can only be used when the wrapped type
    //! is default constructible.
    //!
    //! @note This function is the inverse of `decltype_`.
    constexpr _untype untype{};

    //! Returns the type of an object as a `Type`.
    //! @relates Type
    BOOST_HANA_CONSTEXPR_LAMBDA auto decltype_ = [](auto t) {
        return type<decltype(t)>;
    };

    template <>
    struct Comparable<Type, Type> : defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(operators::_type<T>, operators::_type<U>)
        { return false_; }

        template <typename T>
        static constexpr auto equal_impl(operators::_type<T>, operators::_type<T>)
        { return true_; }
    };


    template <>
    struct Functor<Type> : defaults<Functor> {
        template <typename F, typename T>
        static constexpr auto fmap_impl(F f, operators::_type<T> t)
        { return f(t); }
    };

    template <>
    struct Monad<Type> : defaults<Monad> {
        template <typename T>
        static constexpr operators::_type<T> unit_impl(T)
        { return {}; }

        template <typename T>
        static constexpr operators::_type<T>
        join_impl(operators::_type<operators::_type<T>>)
        { return {}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
