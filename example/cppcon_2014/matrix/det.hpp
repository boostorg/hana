/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_DET_HPP
#define BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_DET_HPP

#include <boost/hana/constant.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <utility>

#include "matrix.hpp"


namespace cppcon {
    namespace detail {
        constexpr int power(int x, unsigned int n) {
            if (n == 0) return 1;
            else        return x * power(x, n - 1);
        }

        auto remove_at = [](auto n, auto xs) {
            using namespace boost::hana;
            using L = datatype_t<decltype(xs)>;
            auto with_indices = zip(xs, to<L>(range(int_<0>, length(xs))));
            auto removed = filter(with_indices, compose(n != _, last));
            return transform(removed, head);
        };
    }

    auto det = boost::hana::fix([](auto det, auto&& m) -> decltype(auto) {
        using namespace boost::hana;
        auto matrix_minor = [=](auto&& m, auto i, auto j) -> decltype(auto) {
            return det(unpack(
                transform(
                    detail::remove_at(i, rows(std::forward<decltype(m)>(m))),
                    partial(detail::remove_at, j)
                ),
                matrix
            ));
        };

        auto cofactor = [=](auto&& m, auto i, auto j) {
            auto i_plus_j = plus(i, j);
            return int_<detail::power(-1, value(i_plus_j))> *
                   matrix_minor(std::forward<decltype(m)>(m), i, j);
        };

        return eval_if(m.size() == int_<1>,
            always(m.at(int_<0>, int_<0>)),
            [=](auto _) {
                auto cofactors_1st_row = unpack(_(range)(int_<0>, m.ncolumns()),
                    on(tuple, partial(cofactor, m, int_<0>))
                );
                return detail::tuple_scalar_product(head(rows(m)), cofactors_1st_row);
            }
        );
    });
} // end namespace cppcon

#endif // !BOOST_HANA_EXAMPLE_CPPCON_2014_MATRIX_DET_HPP
