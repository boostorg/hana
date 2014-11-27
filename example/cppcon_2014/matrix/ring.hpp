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
#include <boost/hana/ring.hpp>
#include <boost/hana/tuple.hpp>

#include <utility>


namespace boost { namespace hana {
    template <unsigned R1, unsigned C, unsigned C2>
    struct Ring::instance<
        cppcon::Matrix<R1, C>, cppcon::Matrix<C, C2>
    > : Ring::mcd {
        template <typename M1, typename M2>
        static constexpr decltype(auto) mult_impl(M1&& m1, M2&& m2) {
            auto cols = cppcon::columns(std::forward<M2>(m2));
            return unpack(
                fmap(cppcon::rows(std::forward<M1>(m1)),
                    [&](auto&& row) -> decltype(auto) {
                        return zip_with(cppcon::detail::tuple_scalar_product,
                            repeat<Tuple>(uint<R1>, std::forward<decltype(row)>(row)),
                            cols
                        );
                    }
                ),
                cppcon::matrix
            );
        }

        static constexpr decltype(auto) one_impl() {
            // todo
        }
    };
}}

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_RING_HPP
