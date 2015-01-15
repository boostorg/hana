/*!
@file
Defines the `BOOST_HANA_DISPATCH_COMMON` macro.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_DISPATCH_COMMON_HPP
#define BOOST_HANA_DETAIL_DISPATCH_COMMON_HPP

// Additional dispatching for binary methods:
//
// 4.1 If `T` and `U` are the same data type, then fail.
//
// 4.2 If `T` and `U` are both a TYPECLASS, and if they have a common type
//     which is also a TYPECLASS, then we use the METHOD of the common type
//     instead.
#define BOOST_HANA_DISPATCH_COMMON(NAME, NAME_IMPL, TYPECLASS)                      \
    template <typename T, typename Context>                                         \
    struct dispatch_impl<4, NAME_IMPL<T, T>, Context> {                             \
        using type = ::boost::hana::not_implemented<NAME_IMPL<T, T>>;               \
    };                                                                              \
                                                                                    \
    template <typename T, typename U, typename Context>                             \
    struct dispatch_impl<4, NAME_IMPL<T, U>, Context> {                             \
        template <typename C>                                                       \
        struct impl {                                                               \
            template <typename X, typename Y>                                       \
            static constexpr decltype(auto) apply(X&& x, Y&& y) {                   \
                return NAME(                                                        \
                    ::boost::hana::to<C>(::boost::hana::detail::std::forward<X>(x)),\
                    ::boost::hana::to<C>(::boost::hana::detail::std::forward<Y>(y)) \
                );                                                                  \
            }                                                                       \
        };                                                                          \
                                                                                    \
        template <typename T_, typename U_,                                         \
                  typename C = typename ::boost::hana::common<T_, U_>::type>        \
        static impl<C> check(::boost::hana::detail::std::integral_constant<bool,    \
            ::boost::hana::is_a<TYPECLASS, T_>() &&                                 \
            ::boost::hana::is_a<TYPECLASS, U_>() &&                                 \
            ::boost::hana::is_a<TYPECLASS, C>()                                     \
        >);                                                                         \
                                                                                    \
        template <typename ...>                                                     \
        static ::boost::hana::not_implemented<NAME_IMPL<T, U>> check(...);          \
                                                                                    \
        using type = decltype(check<T, U>(                                          \
            ::boost::hana::detail::std::true_type{}                                 \
        ));                                                                         \
    }                                                                               \
/**/

#endif // !BOOST_HANA_DETAIL_DISPATCH_COMMON_HPP
