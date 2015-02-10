/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MONOID_HPP
#define BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MONOID_HPP

#include "matrix.hpp"

#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/range.hpp>


namespace boost { namespace hana {
    template <unsigned R, unsigned C>
    struct models<Monoid(cppcon::Matrix<R, C>)>
        : detail::std::true_type
    { };

    template <unsigned R, unsigned C>
    struct plus_impl<cppcon::Matrix<R, C>, cppcon::Matrix<R, C>> {
        template <typename M1, typename M2>
        static constexpr decltype(auto) apply(M1&& m1, M2&& m2) {
            return element_wise(plus)(
                std::forward<M1>(m1),
                std::forward<M2>(m2)
            );
        }
    };

    template <unsigned R, unsigned C>
    struct zero_impl<cppcon::Matrix<R, C>> {
        static constexpr decltype(auto) apply() {
            auto zeros = repeat(int_<C>, int_<0>);
            return unpack(repeat(int_<R>, zeros), cppcon::matrix);
        }
    };
}}

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_MONOID_HPP
