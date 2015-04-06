/*!
@file
Defines `boost::hana::detail::type_foldr1`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_TYPE_FOLDR1_HPP
#define BOOST_HANA_DETAIL_TYPE_FOLDR1_HPP

namespace boost { namespace hana { namespace detail {
    template <unsigned n>
    struct _type_foldr1;

    template <>
    struct _type_foldr1<0> {
        template <
            template <typename ...> class f,
            typename state
        >
        using result = state;
    };

    template <>
    struct _type_foldr1<1> {
        template <
            template <typename ...> class f,
            typename x1,
            typename state
        >
        using result = typename f<x1, state>::type;
    };

    template <>
    struct _type_foldr1<2> {
        template <
            template <typename ...> class f,
            typename x1, typename x2,
            typename state
        >
        using result = typename f<x1, typename f<x2, state>::type>::type;
    };

    template <>
    struct _type_foldr1<3> {
        template <
            template <typename ...> class f,
            typename x1, typename x2, typename x3,
            typename state
        >
        using result = typename f<
            x1,
            typename f<
                x2,
                typename f<
                    x3,
                    state
                >::type
            >::type
        >::type;
    };

    template <>
    struct _type_foldr1<4> {
        template <
            template <typename ...> class f,
            typename x1, typename x2, typename x3, typename x4,
            typename state
        >
        using result = typename f<
            x1,
            typename f<
                x2,
                typename f<
                    x3,
                    typename f<
                        x4,
                        state
                    >::type
                >::type
            >::type
        >::type;
    };

    template <>
    struct _type_foldr1<5> {
        template <
            template <typename ...> class f,
            typename x1, typename x2, typename x3, typename x4, typename x5,
            typename state
        >
        using result = typename f<
            x1,
            typename f<
                x2,
                typename f<
                    x3,
                    typename f<
                        x4,
                        typename f<
                            x5,
                            state
                        >::type
                    >::type
                >::type
            >::type
        >::type;
    };

    template <>
    struct _type_foldr1<6> {
        template <
            template <typename ...> class f,
            typename x1, typename x2, typename x3, typename x4, typename x5, typename x6,
            typename ...xs
        >
        using result =
        typename f<
            x1,
            typename f<
                x2,
                typename f<
                    x3,
                    typename f<
                        x4,
                        typename f<
                            x5,
                            typename _type_foldr1<(sizeof...(xs) > 6 ? 6 : sizeof...(xs))>::
                            template result<f, x6, xs...>
                        >::type
                    >::type
                >::type
            >::type
        >::type;
    };

    template <template <typename ...> class f, typename x1, typename ...xn>
    struct type_foldr1 {
        using type = typename _type_foldr1<(sizeof...(xn) > 6 ? 6 : sizeof...(xn))>
                     ::template result<f, x1, xn...>;
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_TYPE_FOLDR1_HPP
