/*!
@file
Defines `boost::hana::detail::variadic::foldl1`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_FOLDL1_HPP
#define BOOST_HANA_DETAIL_VARIADIC_FOLDL1_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/type/type.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <unsigned int narg>
    struct foldl1_impl;

    template <>
    struct foldl1_impl<1> {
        template <template <typename ...> class F, typename X1>
        using apply_t = X1;

        template <typename F, typename X1>
        static constexpr decltype(auto) apply(F&&, X1&& x1) {
            return detail::std::forward<X1>(x1);
        }
    };

    template <>
    struct foldl1_impl<2> {
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
    struct foldl1_impl<3> {
        template <template <typename ...> class F, typename X1, typename X2, typename X3>
        using apply_t = typename F<typename F<X1, X2>::type, X3>::type;

        template <typename F, typename X1, typename X2, typename X3>
        static constexpr decltype(auto) apply(F&& f, X1&& x1, X2&& x2, X3&& x3) {
            return f(
                f(
                    detail::std::forward<X1>(x1),
                    detail::std::forward<X2>(x2)
                ),
                detail::std::forward<X3>(x3)
            );
        }
    };

    // Given a number of arguments left to process, returns the number with
    // which the next call to `foldl1_impl` should be specialized.
    constexpr unsigned int foldl1_next(unsigned int n)
    { return n > 4 ? 4 : n; }

    template <>
    struct foldl1_impl<4> {
        template <template <typename ...> class F, typename X1, typename X2, typename X3, typename X4, typename ...Xs>
        using apply_t = typename foldl1_impl<foldl1_next(sizeof...(Xs) + 1)>::
                        template apply_t<F, typename F<typename F<typename F<X1, X2>::type, X3>::type, X4>::type, Xs...>;

        template <typename F, typename X1, typename X2, typename X3, typename X4, typename ...Xs>
        static constexpr decltype(auto) apply(F&& f, X1&& x1, X2&& x2, X3&& x3, X4&& x4, Xs&& ...xs) {
            return foldl1_impl<foldl1_next(sizeof...(Xs) + 1)>::
                   apply(f,
                    f(
                        f(
                            f(
                                detail::std::forward<X1>(x1),
                                detail::std::forward<X2>(x2)
                            ),
                            detail::std::forward<X3>(x3)
                        ),
                        detail::std::forward<X4>(x4)
                    ),
                    detail::std::forward<Xs>(xs)...
                );
        }
    };

    template <typename ...Xs, typename F>
    constexpr decltype(auto) foldl1_helper(F&& f, ...) {
        return foldl1_impl<foldl1_next(sizeof...(Xs))>::apply(
            detail::std::forward<F>(f), type<Xs>...
        );
    }

    template <typename ...Xs, typename F>
    constexpr auto foldl1_helper(F, Metafunction*) {
        return type<
            typename foldl1_impl<foldl1_next(sizeof...(Xs))>::
            template apply_t<F::template apply, Xs...>
        >;
    }

    template <typename ...Xs, typename F>
    constexpr decltype(auto) foldl1(F&& f) {
        return foldl1_helper<Xs...>(
            detail::std::forward<F>(f), (datatype_t<F>*)nullptr
        );
    }

    template <typename F, typename ...Xs>
    constexpr decltype(auto) foldl1(F&& f, Xs&& ...xs) {
        return foldl1_impl<foldl1_next(sizeof...(Xs))>::apply(
            detail::std::forward<F>(f),
            detail::std::forward<Xs>(xs)...
        );
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOLDL1_HPP
