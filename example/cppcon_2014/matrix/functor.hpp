/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_FUNCTOR_HPP
#define BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_FUNCTOR_HPP

#include "matrix.hpp"

#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>

#include <utility>


namespace boost { namespace hana {
    template <unsigned Rows, unsigned Columns>
    struct Functor::instance<cppcon::Matrix<Rows, Columns>> : Functor::fmap_mcd {
        template <typename M, typename F>
        static constexpr decltype(auto) fmap_impl(M&& m, F&& f) {
            return unpack(
                fmap(
                    cppcon::rows(std::forward<M>(m)),
                    partial(flip(fmap), std::forward<F>(f))
                ),
                cppcon::matrix
            );
        }
    };
}}

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_FUNCTOR_HPP
