/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP
#define BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/comparable_from_iterable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/foldable_from_iterable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


namespace {
    template <typename T, typename Datatype>
    struct with_datatype_impl {
        T actual;
        using hana_datatype = Datatype;
    };

    struct MinimalIterable;

    BOOST_HANA_CONSTEXPR_LAMBDA auto iterable = [](auto ...xs) {
        return with_datatype_impl<
            decltype(boost::hana::list(xs...)), MinimalIterable
        >{boost::hana::list(xs...)};
    };
}

namespace boost { namespace hana {
    template <>
    struct Iterable<MinimalIterable> : defaults<Iterable> {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs)
        { return head(xs.actual); }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return with_datatype_impl<
                decltype(tail(xs.actual)), MinimalIterable
            >{tail(xs.actual)};
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs)
        { return is_empty(xs.actual); }
    };

    template <>
    struct Foldable<MinimalIterable>
        : detail::foldable_from_iterable
    { };

    template <>
    struct Comparable<MinimalIterable, MinimalIterable>
        : detail::comparable_from_iterable
    { };
}}

#endif // !BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP
