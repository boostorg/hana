/*!
@file
Defines `boost::hana::dispatch` `boost::hana::context` and
`boost::hana::not_implemented`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_DISPATCH_HPP
#define BOOST_HANA_CORE_DISPATCH_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Use this as a base class of a method to mark it as disabled.
    //!
    //! ...
    //!
    //! ### Example
    //! @snippet example/core/dispatch.cpp not_implemented
    template <typename Method>
    struct not_implemented { };

    namespace core_detail {
        template <typename ...>
        struct holder { };
    }

    //! @ingroup group-core
    //! ...
    template <typename ...Visiting>
    struct context : core_detail::holder<Visiting>... {
        template <typename Method>
        static constexpr bool contains_impl(core_detail::holder<Method>)
        { return true; }
        template <typename Method>
        static constexpr bool contains_impl(...)
        { return false; }
        template <typename Method>
        struct contains {
            static constexpr bool value = contains_impl<Method>(context{});
        };


        template <typename Method>
        static context add_impl(core_detail::holder<Method>*);
        template <typename Method>
        static context<Method, Visiting...> add_impl(...);
        template <typename Method>
        using add = decltype(add_impl<Method>((context*)0));
    };

    template <>
    struct context<> {
        template <typename Method>
        struct contains {
            static constexpr bool value = false;
        };
        template <typename Method>
        using add = context<Method>;
    };

    //! @ingroup group-core
    //! ...
    //!
    //! @todo
    //! Explain the multi-level dispatching, how it can be customized and
    //! what is the Context.
    template <unsigned Level, typename Method, typename Context, typename = void>
    struct dispatch_impl;

    template <typename Method, typename Context>
    struct dispatch_impl<1, Method, Context, decltype((void)sizeof(Method))> {
        using type = Method;
    };
    template <typename Method, typename Context, typename Otherwise>
    struct dispatch_impl<1, Method, Context, Otherwise>
        : dispatch_impl<2, Method, Context>
    { };


    template <template <typename ...> class Method, typename ...T, typename Context>
    struct dispatch_impl<2, Method<T...>, Context, decltype((void)sizeof(Method<T..., when<true>>))> {
        using type = Method<T..., when<true>>;
    };
    template <template <typename ...> class Method, typename ...T, typename Context, typename Otherwise>
    struct dispatch_impl<2, Method<T...>, Context, Otherwise>
        : dispatch_impl<3, Method<T...>, Context>
    { };


    template <template <typename ...> class Method, typename ...T, typename Context>
    struct dispatch_impl<3, Method<T...>, Context, decltype((void)sizeof(Method<T..., when<true>, Context>))> {
        using type = Method<T..., when<true>, Context>;
    };
    template <template <typename ...> class Method, typename ...T, typename Context, typename Otherwise>
    struct dispatch_impl<3, Method<T...>, Context, Otherwise>
        : dispatch_impl<4, Method<T...>, Context>
    { };


    template <template <typename ...> class Method, typename ...T, typename Context>
    struct dispatch_impl<4, Method<T...>, Context> {
        using type = not_implemented<Method<T...>>;
    };


    //! @ingroup group-core
    //! ...
    //!
    //! ...
    //!
    //! ### Example
    //! @snippet example/core/dispatch.cpp dispatch
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Method, typename Context = context<>>
    struct dispatch;
#else
    template <typename Method, typename Context = context<>, bool = Context::template contains<Method>::value>
    struct dispatch
        : not_implemented<Method>
    { };

    template <typename Method, typename Context>
    struct dispatch<Method, Context, false>
        : dispatch_impl<1, Method, typename Context::template add<Method>>::type
    { };
#endif

    //! @ingroup group-core
    //! Defines a unary method.
    //!
    //! ...
    //!
    //! ### Example
    //! @include example/core/method.cpp unary_method
    #define BOOST_HANA_METHOD(NAME_IMPL)                                    \
        template <typename T, typename ...>                                 \
        struct NAME_IMPL                                                    \
    /**/

    //! @ingroup group-core
    //! Defines a binary method.
    //!
    //! This is equivalent to `BOOST_HANA_METHOD`, except it creates a method
    //! using two data types for tag dispatching.
    //!
    //! ### Example
    //! @include example/core/method.cpp binary_method
    #define BOOST_HANA_BINARY_METHOD(NAME_IMPL)                             \
        template <typename T, typename U, typename ...>                     \
        struct NAME_IMPL                                                    \
    /**/
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_DISPATCH_HPP
