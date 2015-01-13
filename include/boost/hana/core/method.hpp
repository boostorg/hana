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
    template <template <typename ...> class Method, typename ...T>
    struct not_implemented {
        template <typename ...X>
        static constexpr void apply(X const& ...) {
            static_assert(sizeof...(X) && false,
            "the method is not implemented for the given data type(s)");
        }
    };

    namespace core_detail {
        template <template <typename ...> class M, typename ...T>
        struct method;

        template <typename Method>
        struct methodify;

        template <template <typename ...> class M, typename T, typename Unavailable, typename When>
        struct methodify<M<T, Unavailable, When>> {
            using type = method<M, T>;
        };

        template <template <typename ...> class M, typename T, typename U, typename Unavailable, typename When>
        struct methodify<M<T, U, Unavailable, When>> {
            using type = method<M, T, U>;
        };


        template <template <typename ...> class M, typename ...T>
        constexpr bool inherits_not_implemented(not_implemented<M, T...>) { return true; }
        constexpr bool inherits_not_implemented(...) { return false; }

        /////////// Temporary and naive implementation
        template <typename Method, typename Unavailable>
        struct is_in { static constexpr bool value = false; };

        template <typename Method, typename U, typename ...Us>
        struct is_in<Method, unavailable<U, Us...>>
            : is_in<Method, unavailable<Us...>>
        { };

        template <typename Method, typename ...Us>
        struct is_in<Method, unavailable<Method, Us...>>
        { static constexpr bool value = true; };
        //////////////////////////////////////////////////////////////////////

        template <typename Method, typename Unavailable, bool = is_in<Method, Unavailable>::value>
        struct is_implemented_impl;

        template <template <typename ...> class M, typename ...T, typename ...Unavailable>
        struct is_implemented_impl<method<M, T...>, unavailable<Unavailable...>, true> {
            static constexpr bool value = false;
        };

        template <template <typename ...> class M, typename ...T, typename ...Unavailable>
        struct is_implemented_impl<method<M, T...>, unavailable<Unavailable...>, false> {
            static constexpr bool value = !inherits_not_implemented(
                M<T..., unavailable<method<M, T...>, Unavailable...>, void>{}
            );
        };
    } // end namespace core_detail

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
    constexpr bool is_implemented = core_detail::is_implemented_impl<
        typename core_detail::methodify<Method>::type, Unavailable
    >::value;
#endif

    //! @ingroup group-core
    //! Defines a unary method.
    //!
    //! ...
    //!
    //! ### Example
    //! @include example/core/method.cpp unary_method
    #define BOOST_HANA_METHOD(NAME)                                         \
        /** @cond */                                                        \
        template <typename, typename, typename>                             \
        struct NAME;                                                        \
                                                                            \
        template <                                                          \
            typename T,                                                     \
            typename _ = ::boost::hana::unavailable<                        \
                            ::boost::hana::core_detail::method<NAME, T>>,   \
            typename = void>                                                \
        struct NAME                                                         \
            : NAME<T, _, ::boost::hana::when<true>>                         \
        { };                                                                \
                                                                            \
        template <typename T, typename _, bool condition>                   \
        struct NAME<T, _, ::boost::hana::when<condition>>                   \
            : ::boost::hana::not_implemented<NAME, T>                       \
        { }                                                                 \
        /** @endcond */                                                     \
    /**/

    //! @ingroup group-core
    //! Defines a binary method.
    //!
    //! This is equivalent to `BOOST_HANA_METHOD`, except it creates a method
    //! using two data types for tag dispatching.
    //!
    //! ### Example
    //! @include example/core/method.cpp binary_method
    #define BOOST_HANA_BINARY_METHOD(NAME)                                  \
        /** @cond */                                                        \
        template <typename, typename, typename, typename>                   \
        struct NAME;                                                        \
                                                                            \
        template <                                                          \
            typename T, typename U,                                         \
            typename _ = ::boost::hana::unavailable<                        \
                            ::boost::hana::core_detail::method<NAME, T, U>>,\
            typename = void>                                                \
        struct NAME                                                         \
            : NAME<T, U, _, ::boost::hana::when<true>>                      \
        { };                                                                \
                                                                            \
        template <typename T, typename U, typename _, bool condition>       \
        struct NAME<T, U, _, ::boost::hana::when<condition>>                \
            : ::boost::hana::not_implemented<NAME, T, U>                    \
        { }                                                                 \
        /** @endcond */                                                     \
    /**/
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_METHOD_HPP
