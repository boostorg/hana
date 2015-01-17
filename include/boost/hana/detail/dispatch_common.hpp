/*!
@file
Defines `boost::hana::detail::dispatch_common`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_DISPATCH_COMMON_HPP
#define BOOST_HANA_DETAIL_DISPATCH_COMMON_HPP

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Dispatching plugin for binary methods.
    //!
    //! Specifically, if `T` and `U` are the same data type, then fail.
    //! Otherwise, if `T` and `U` are both models of `Typeclass`, and if
    //! they have a common type which is also a model of `Typeclass`, then
    //! dispatch the `Method` to that common type instead.
    template <typename Method, typename Typeclass, typename Context>
    struct dispatch_common;

    template <template <typename ...> class Method, typename T, typename U, typename Typeclass, typename Context>
    struct dispatch_common<Method<T, U>, Typeclass, Context> {
        template <typename C>
        struct impl {
            template <typename X, typename Y>
            static constexpr decltype(auto) apply(X&& x, Y&& y) {
                return dispatch<Method<C, C>>::apply(
                    to<C>(detail::std::forward<X>(x)),
                    to<C>(detail::std::forward<Y>(y))
                );
            }
        };

        template <typename T_, typename U_,
                  typename C = typename common<T_, U_>::type>
        static impl<C> check(detail::std::integral_constant<bool,
            is_a<Typeclass, T_>() && is_a<Typeclass, U_>() && is_a<Typeclass, C>()
        >);

        template <typename ...>
        static not_implemented<Method<T, U>> check(...);

        using type = decltype(check<T, U>(detail::std::true_type{}));
    };

    template <template <typename ...> class Method, typename T, typename Typeclass, typename Context>
    struct dispatch_common<Method<T, T>, Typeclass, Context> {
        using type = not_implemented<Method<T, T>>;
    };
}}}

#endif // !BOOST_HANA_DETAIL_DISPATCH_COMMON_HPP
