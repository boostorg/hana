/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_SEQ_ITERABLE_HPP
#define BOOST_HANA_TEST_TEST_SEQ_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/iterable.hpp>
#include <test/seq/seq.hpp>


namespace boost { namespace hana {
    template <>
    struct Iterable::instance<test::Seq> : Iterable::mcd {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs) {
            return xs.storage([=](auto x, auto ...xs) {
                return x;
            });
        }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return xs.storage([=](auto x, auto ...xs) {
                return test::seq(xs...);
            });
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs) {
            return xs.storage([=](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_SEQ_ITERABLE_HPP
