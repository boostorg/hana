/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MONOID_HPP
#define BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MONOID_HPP

#include "matrix.hpp"

#include <boost/hana/integral.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/monoid/mcd.hpp>
#include <boost/hana/range/foldable.hpp>


namespace boost { namespace hana {
    template <unsigned Rows, unsigned Columns>
    struct Monoid::instance<
        cppcon::Matrix<Rows, Columns>, cppcon::Matrix<Rows, Columns>
    > : Monoid::mcd {
        template <typename M1, typename M2>
        static constexpr decltype(auto) plus_impl(M1&& m1, M2&& m2) {
            return element_wise(plus)(
                std::forward<M1>(m1),
                std::forward<M2>(m2)
            );
        }

        static constexpr decltype(auto) zero_impl() {
            auto zeros = repeat(int_<Columns>, int_<0>);
            return unpack(repeat(int_<Rows>, zeros), cppcon::matrix);
        }
    };
}}

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MONOID_HPP
