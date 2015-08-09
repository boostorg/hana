/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/monadic_fold_right.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto safe_div = [](auto x, auto y) {
        return hana::eval_if(y == hana::int_<0>,
            hana::lazy(hana::nothing),
            [=](auto _) {
                return hana::just(_(x) / y);
            }
        );
    };

    // with an initial state
    BOOST_HANA_CONSTANT_CHECK(
        hana::monadic_fold_right<hana::Optional>(
            hana::tuple_c<int, 1000, 8, 4>, hana::int_<2>, safe_div
        )
            ==
        hana::just(hana::int_<1000> / (hana::int_<8> / (hana::int_<4> / hana::int_<2>)))
    );

    BOOST_HANA_CONSTANT_CHECK(
        hana::monadic_fold_right<hana::Optional>(
            hana::tuple_c<int, 1000, 8, 4>, hana::int_<0>, safe_div
        )
            ==
        hana::nothing
    );

    // without an initial state
    BOOST_HANA_CONSTANT_CHECK(
        hana::monadic_fold_right<hana::Optional>(
            hana::tuple_c<int, 1000, 8, 4, 2>, safe_div
        )
            ==
        hana::just(hana::int_<1000> / (hana::int_<8> / (hana::int_<4> / hana::int_<2>)))
    );

    BOOST_HANA_CONSTANT_CHECK(
        hana::monadic_fold_right<hana::Optional>(
            hana::tuple_c<int, 1000, 8, 4, 0>, safe_div
        )
            ==
        hana::nothing
    );
}
