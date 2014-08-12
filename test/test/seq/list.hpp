/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_SEQ_LIST_HPP
#define BOOST_HANA_TEST_TEST_SEQ_LIST_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/mcd.hpp>
#include <test/seq/seq.hpp>

// MCD
#include <test/seq/foldable.hpp>
#include <test/seq/iterable.hpp>
#include <test/seq/monad.hpp>


namespace boost { namespace hana {
    template <>
    struct List::instance<test::Seq> : List::mcd<test::Seq> {
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return test::seq(x, xs...);
            });
        }

        static BOOST_HANA_CONSTEXPR_LAMBDA auto nil_impl() {
            return test::seq();
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_SEQ_LIST_HPP
