/*!
@file
Defines `boost::hana::detail::variadic::foldr1`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_FOLDR1_HPP
#define BOOST_HANA_DETAIL_VARIADIC_FOLDR1_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/fwd/type.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <unsigned int narg>
    struct foldr1_impl;

    template <>
    struct foldr1_impl<1> {
        template <template <typename ...> class F, typename X1>
        using apply_t = X1;

        template <typename F, typename X1>
        static constexpr decltype(auto) apply(F&&, X1&& x1) {
            // id handles rvalue-ness properly
            return id(detail::std::forward<X1>(x1));
        }
    };

    template <>
    struct foldr1_impl<2> {
        template <template <typename ...> class F, typename X1, typename X2>
        using apply_t = typename F<X1, X2>::type;

        template <typename F, typename X1, typename X2>
        static constexpr decltype(auto) apply(F&& f, X1&& x1, X2&& x2) {
            return detail::std::forward<F>(f)(
                detail::std::forward<X1>(x1),
                detail::std::forward<X2>(x2)
            );
        }
    };

    template <>
    struct foldr1_impl<3> {
        template <template <typename ...> class F, typename X1, typename X2, typename X3>
        using apply_t = typename F<X1, typename F<X2, X3>::type>::type;

        template <typename F, typename X1, typename X2, typename X3>
        static constexpr decltype(auto) apply(F&& f, X1&& x1, X2&& x2, X3&& x3) {
            return f(
                detail::std::forward<X1>(x1),
                f(
                    detail::std::forward<X2>(x2),
                    detail::std::forward<X3>(x3)
                )
            );
        }
    };

    // Given a number of arguments left to process, returns the number with
    // which the next call to `foldr1_impl` should be specialized.
    constexpr unsigned int foldr1_next(unsigned int n)
    { return n > 4 ? 4 : n; }

    template <>
    struct foldr1_impl<4> {
        template <template <typename ...> class F, typename X1, typename X2, typename X3, typename ...Xs>
        using apply_t = typename F<X1, typename F<X2, typename F<X3,
            typename foldr1_impl<foldr1_next(sizeof...(Xs))>::
            template apply_t<F, Xs...>
        >::type>::type>::type;

        template <typename F, typename X1, typename X2, typename X3, typename ...Xs>
        static constexpr decltype(auto) apply(F&& f, X1&& x1, X2&& x2, X3&& x3, Xs&& ...xs) {
            return f(
                detail::std::forward<X1>(x1),
                f(
                    detail::std::forward<X2>(x2),
                    f(
                        detail::std::forward<X3>(x3),
                        foldr1_impl<foldr1_next(sizeof...(Xs))>::apply(
                            f,
                            detail::std::forward<Xs>(xs)...
                        )
                    )
                )
            );
        }
    };

    template <typename ...Xs, typename F>
    constexpr decltype(auto) foldr1_helper(F&& f, ...) {
        return foldr1_impl<foldr1_next(sizeof...(Xs))>::apply(
            detail::std::forward<F>(f), type<Xs>...
        );
    }

    template <typename ...Xs, typename F>
    constexpr auto foldr1_helper(F, Metafunction*) {
        return type<
            typename foldr1_impl<foldr1_next(sizeof...(Xs))>::
            template apply_t<F::template apply, Xs...>
        >;
    }

    template <typename ...Xs, typename F>
    constexpr decltype(auto) foldr1_t(F&& f) {
        return foldr1_helper<Xs...>(
            detail::std::forward<F>(f), (datatype_t<F>*)nullptr
        );
    }

    struct _foldr1 {
        template <typename F, typename ...Xs>
        constexpr decltype(auto) operator()(F&& f, Xs&& ...xs) const {
            return foldr1_impl<foldr1_next(sizeof...(Xs))>::apply(
                detail::std::forward<F>(f),
                detail::std::forward<Xs>(xs)...
            );
        }
    };

    constexpr _foldr1 foldr1{};
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDR1_HPP
