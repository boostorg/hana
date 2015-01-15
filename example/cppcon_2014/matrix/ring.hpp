/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_RING_HPP
#define BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_RING_HPP

#include "matrix.hpp"

#include <boost/hana/foldable.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/ring.hpp>
#include <boost/hana/tuple.hpp>

#include <utility>


namespace boost { namespace hana {
    template <unsigned M, unsigned N, unsigned K>
    struct mult_impl<cppcon::Matrix<M, N>, cppcon::Matrix<N, K>> {
        template <typename A, typename B>
        static constexpr decltype(auto) apply(A&& a, B&& b) {
            auto cols = cppcon::columns(std::forward<B>(b));
            return unpack(
                fmap(cppcon::rows(std::forward<A>(a)),
                    [&](auto&& row) -> decltype(auto) {
                        return zip_with(cppcon::detail::tuple_scalar_product,
                            repeat<Tuple>(uint<M>, std::forward<decltype(row)>(row)),
                            cols
                        );
                    }
                ),
                cppcon::matrix
            );
        }
    };

    template <unsigned M, unsigned N>
    struct one_impl<cppcon::Matrix<M, N>> {
        static constexpr decltype(auto) apply() {
            return unpack(range_c<unsigned, 0, M>, [](auto ...n) {
                return unpack(range_c<unsigned, 0, N>, [=](auto ...m) {
                    auto row = [=](auto n) {
                        return cppcon::row(if_(n == m, int_<1>, int_<0>)...);
                    };
                    return cppcon::matrix(row(n)...);
                });
            });
        }
    };
}}

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_RING_HPP
