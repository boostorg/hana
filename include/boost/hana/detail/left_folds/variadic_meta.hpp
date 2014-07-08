/*!
@file
Defines `boost::hana::detail::left_folds::variadic_meta`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//////////////////////////////////////////////////////////////////////////
// GENERATED HEADER: DO NOT EDIT.
//////////////////////////////////////////////////////////////////////////

#ifndef BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_META_HPP
#define BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_META_HPP

namespace boost { namespace hana { namespace detail { namespace left_folds {
    template <unsigned n>
    struct variadic_meta_impl;

    template <>
    struct variadic_meta_impl<    6   > {
        template <template <typename ...> class f, typename state, typename x1, typename x2, typename x3, typename x4, typename x5, typename x6, typename ...xs>
        using result =
            typename variadic_meta_impl<  (sizeof...(xs) > 6 ? 6 : sizeof...(xs))    >::
            template result<f, f<f<f<f<f<f<state, x1>, x2>, x3>, x4>, x5>, x6>, xs...>;
    };

    template <>
    struct variadic_meta_impl<    0    > {
        template <template <typename ...> class f, typename state>
        using result = state;
    };

    template <>
    struct variadic_meta_impl<    1    > {
        template <template <typename ...> class f, typename state, typename x1>
        using result = f<state, x1>;
    };

    template <>
    struct variadic_meta_impl<    2    > {
        template <template <typename ...> class f, typename state, typename x1, typename x2>
        using result = f<f<state, x1>, x2>;
    };

    template <>
    struct variadic_meta_impl<    3    > {
        template <template <typename ...> class f, typename state, typename x1, typename x2, typename x3>
        using result = f<f<f<state, x1>, x2>, x3>;
    };

    template <>
    struct variadic_meta_impl<    4    > {
        template <template <typename ...> class f, typename state, typename x1, typename x2, typename x3, typename x4>
        using result = f<f<f<f<state, x1>, x2>, x3>, x4>;
    };

    template <>
    struct variadic_meta_impl<    5    > {
        template <template <typename ...> class f, typename state, typename x1, typename x2, typename x3, typename x4, typename x5>
        using result = f<f<f<f<f<state, x1>, x2>, x3>, x4>, x5>;
    };

    template <template <typename ...> class f, typename state, typename ...xs>
    struct variadic_meta_indirect {
        using type = typename variadic_meta_impl<(sizeof...(xs) > 6 ? 6 : sizeof...(xs))>::
                     template result<f, state, xs...>;
    };

    template <template <typename ...> class f>
    struct evaluator {
        template <typename ...x>
        using apply = typename f<x...>::type;
    };


    /*!
     * @ingroup details
     *
     * Recursive alias-based variadic left fold for types only.
     */
    template <template <typename ...> class f, typename state, typename ...xs>
    using variadic_meta = typename variadic_meta_indirect<evaluator<f>::template apply, state, xs...>::type;
}}}} // end namespace boost::hana::detail::left_folds

#endif // !BOOST_HANA_DETAIL_LEFT_FOLDS_VARIADIC_META_HPP
