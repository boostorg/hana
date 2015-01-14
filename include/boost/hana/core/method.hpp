/*!
@file
Contains utilities to define tag-dispatched methods.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_METHOD_HPP
#define BOOST_HANA_CORE_METHOD_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Used to specify methods that must be ignored when tag-dispatching a
    //! method.
    //!
    //! ...
    //!
    //! ### Example
    //! @include example/core/method.cpp unavailable
    template <typename ...Methods>
    struct unavailable;

    //! @ingroup group-core
    //! Use this as a base class of a method to mark it as disabled.
    //!
    //! ...
    //!
    //! ### Example
    //! @include example/core/method.cpp not_implemented
    template <typename Method>
    struct not_implemented {
        template <typename ...X>
        static constexpr void apply(X const& ...) {
            static_assert(sizeof...(X) && false,
            "the method is not implemented for the given data type(s)");
        }
    };

    namespace dispatch_detail {
        /////////// is_in
        template <typename Method, typename Unavailable>
        struct is_in { static constexpr bool value = false; };

        template <typename Method, typename U, typename ...Us>
        struct is_in<Method, unavailable<U, Us...>>
            : is_in<Method, unavailable<Us...>>
        { };

        template <typename Method, typename ...Us>
        struct is_in<Method, unavailable<Method, Us...>>
        { static constexpr bool value = true; };

        /////////// either
        template <typename T, typename = void>
        struct either {
            template <typename U> using lazy_or = U;
            template <typename U> struct or_ { using type = U; };
        };

        template <typename T>
        struct either<T, decltype((void)sizeof(T))> {
            template <typename U> struct lazy_or { using type = T; };
            template <typename U> struct or_ { using type = T; };
        };

        /////////// inherits_not_implemented
        template <typename M>
        constexpr bool inherits_not_implemented(not_implemented<M>) { return true; }
        constexpr bool inherits_not_implemented(...) { return false; }
    }

    //! @ingroup group-core
    //! ...
    //!
    //! @todo
    //! Explain the multi-level dispatching, how it can be customized and
    //! what is the Context.
    template <unsigned Level, typename Method, typename Context>
    struct dispatch_impl;

    template <template <typename ...> class Method, typename ...T, typename Context>
    struct dispatch_impl<1, Method<T...>, Context>
        : dispatch_detail::either<Method<T...>>::template
          lazy_or<dispatch_impl<2, Method<T...>, Context>>
    { };

    template <template <typename ...> class Method, typename ...T, typename Context>
    struct dispatch_impl<2, Method<T...>, Context>
        : dispatch_detail::either<Method<T..., when<true>>>::template
          lazy_or<dispatch_impl<3, Method<T...>, Context>>
    { };

    template <template <typename ...> class Method, typename ...T, typename Context>
    struct dispatch_impl<3, Method<T...>, Context>
        : dispatch_detail::either<Method<T..., when<true>, Context>>::template
          lazy_or<dispatch_impl<4, Method<T...>, Context>>
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
    //! @include example/core/method.cpp dispatch
    //!
    //! @todo Perhaps rename this to `method`?
    template <typename MethodImpl, typename Unavailable = unavailable<>, bool = dispatch_detail::is_in<MethodImpl, Unavailable>::value>
    struct dispatch;

    template <template <typename ...> class MethodImpl, typename ...T, typename ...Unavailable>
    struct dispatch<MethodImpl<T...>, unavailable<Unavailable...>, false>
        : dispatch_impl<1,
            MethodImpl<T...>,
            unavailable<MethodImpl<T...>, Unavailable...>
        >::type
    { };

    template <typename MethodImpl, typename Unavailable>
    struct dispatch<MethodImpl, Unavailable, true>
        : not_implemented<MethodImpl>
    { };

    //! @ingroup group-core
    //! Returns whether a method is implemented for the given data type(s).
    //!
    //! ...
    //!
    //! ### Example
    //! @include example/core/method.cpp is_implemented
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Method, typename Unavailable = unavailable<>>
    constexpr bool is_implemented = whether Method has an implementation that does not require any of the Unavailable methods;
#else
    template <typename Method, typename Unavailable = unavailable<>>
    constexpr bool is_implemented = !dispatch_detail::inherits_not_implemented(
        dispatch<Method, Unavailable>{}
    );
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

#endif // !BOOST_HANA_CORE_METHOD_HPP
