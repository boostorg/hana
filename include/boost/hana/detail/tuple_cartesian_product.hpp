/*!
@file
Defines `boost::hana::detail::tuple_cartesian_product`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_TUPLE_CARTESIAN_PRODUCT_HPP
#define BOOST_HANA_DETAIL_TUPLE_CARTESIAN_PRODUCT_HPP

#include <boost/hana/detail/array.hpp>
#include <boost/hana/detail/generate_integer_sequence.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana { namespace detail {
    struct _tuple_cartesian_product {
        using Size = detail::std::size_t;

        template <unsigned Which, Size ...>
        struct indexN;

        template <unsigned Which, Size I, Size J>
        struct indexN<Which, I, J> {
            constexpr auto operator()(...) const {
                detail::array<detail::array<Size, I * J>, 2> arrays{};
                for (Size i = 0, index = 0; i < I; ++i) {
                    for (Size j = 0; j < J; ++j, ++index) {
                        arrays[0][index] = i;
                        arrays[1][index] = j;
                    }
                }
                return arrays[Which];
            }
        };

        template <unsigned Which, Size I, Size J, Size K>
        struct indexN<Which, I, J, K> {
            constexpr auto operator()(...) const {
                detail::array<detail::array<Size, I * J * K>, 3> arrays{};
                for (Size i = 0, index = 0; i < I; ++i) {
                    for (Size j = 0; j < J; ++j) {
                        for (Size k = 0; k < K; ++k, ++index) {
                            arrays[0][index] = i;
                            arrays[1][index] = j;
                            arrays[2][index] = k;
                        }
                    }
                }
                return arrays[Which];
            }
        };

        template <typename ...T, typename ...U,
                  Size ...i,     Size ...j>
        static constexpr auto
        cartesian_prod_impl(_tuple<T...> const& a,
                            _tuple<U...> const& b,
                            detail::std::index_sequence<i...>,
                            detail::std::index_sequence<j...>)
        {
            return hana::make_tuple(hana::make_tuple(
                detail::get<i>(a), detail::get<j>(b)
            )...);
        }

        template <typename ...T, typename ...U, typename ...V,
                 Size ...i,      Size ...j,     Size ...k>
        static constexpr auto
        cartesian_prod_impl(_tuple<T...> const& a,
                            _tuple<U...> const& b,
                            _tuple<V...> const& c,
                            detail::std::index_sequence<i...>,
                            detail::std::index_sequence<j...>,
                            detail::std::index_sequence<k...>)
        {
            return hana::make_tuple(hana::make_tuple(
                detail::get<i>(a), detail::get<j>(b), detail::get<k>(c)
            )...);
        }

        template <typename ...T, typename ...U,
                  Size ...i,     Size ...j>
        static constexpr auto
        cartesian_prod_impl_cat2(_tuple<T...> const& a,
                            _tuple<U...> const& b,
                            detail::std::index_sequence<i...>,
                            detail::std::index_sequence<j...>)
        {
            return hana::make_tuple(hana::concat(
                detail::get<i>(a), detail::get<j>(b)
            )...);
        }

        template <typename ...Tuples>
        static constexpr decltype(auto)
        cartesian_prod_helper_cat2(Tuples&& ...tuples) {
            constexpr long long lengths[] = {tuple_detail::size<Tuples>{}...};
            constexpr Size total_length = hana::product(lengths);
            return cartesian_prod_impl_cat2(
                static_cast<Tuples&&>(tuples)...,
                detail::generate_index_sequence<total_length,
                    indexN<0, tuple_detail::size<Tuples>{}...>
                >{},
                detail::generate_index_sequence<total_length,
                    indexN<1, tuple_detail::size<Tuples>{}...>
                >{}
            );
        }

        template <Size ...i, typename ...Tuples>
        static constexpr decltype(auto)
        cartesian_prod_helper(detail::std::index_sequence<i...>, Tuples&& ...tuples) {
            constexpr long long lengths[] = {tuple_detail::size<Tuples>{}...};
            constexpr Size total_length = hana::product(lengths);
            return cartesian_prod_impl(
                static_cast<Tuples&&>(tuples)...,
                detail::generate_index_sequence<total_length,
                    indexN<i, tuple_detail::size<Tuples>{}...>
                >{}...
            );
        }

        template <typename Tuple>
        constexpr decltype(auto) operator()(Tuple&& tuple) const
        { return hana::transform(static_cast<Tuple&&>(tuple), hana::make_tuple); }

        template <typename ...Tuples>
        constexpr decltype(auto) operator()(Tuples&& ...tuples) const {
            static_assert(sizeof...(Tuples) == 2 || sizeof...(Tuples) == 3,
            "this overload should only pick up 2 and 3 tuples");

            return cartesian_prod_helper(
                detail::std::make_index_sequence<sizeof...(Tuples)>{},
                static_cast<Tuples&&>(tuples)...
            );
        }

        template <typename A, typename B, typename C, typename D1, typename ...Dn>
        constexpr decltype(auto)
        operator()(A&& a, B&& b, C&& c, D1&& d1, Dn&& ...dn) const {
            return hana::transform( // Flatten to get A x B x C x D1 x ... x Dn
                cartesian_prod_helper_cat2( // <- Compute (A x B x C) x (D1 x ... x Dn)
                    (*this)( // <- Compute A x B x C
                        static_cast<A&&>(a),
                        static_cast<B&&>(b),
                        static_cast<C&&>(c)
                    ),
                    (*this)( // <- Compute D1 x ... x Dn
                        static_cast<D1&&>(d1),
                        static_cast<Dn&&>(dn)...
                    )
                )
            , id);
        }
    };
    constexpr _tuple_cartesian_product tuple_cartesian_product{};
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_TUPLE_CARTESIAN_PRODUCT_HPP
