/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP
#define BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


struct MinimalIterable;

BOOST_HANA_CONSTEXPR_LAMBDA auto iterable = [](auto ...xs) {
    return boost::hana::detail::wrap<MinimalIterable>(boost::hana::list(xs...));
};

struct MinimalInstance : boost::hana::Iterable::mcd {
    template <typename Xs>
    static constexpr auto head_impl(Xs xs)
    { return boost::hana::head(boost::hana::detail::unwrap(xs)); }

    template <typename Xs>
    static constexpr auto tail_impl(Xs xs) {
        return boost::hana::detail::wrap<MinimalIterable>(
            boost::hana::tail(boost::hana::detail::unwrap(xs))
        );
    }

    template <typename Xs>
    static constexpr auto is_empty_impl(Xs xs)
    { return boost::hana::is_empty(boost::hana::detail::unwrap(xs)); }
};

namespace boost { namespace hana {
    template <>
    struct Iterable::instance<MinimalIterable>
        : MinimalInstance
    { };

    template <>
    struct Comparable::instance<MinimalIterable, MinimalIterable>
        : Iterable::ComparableInstance
    { };
}}

#endif // !BOOST_HANA_TEST_ITERABLE_MINIMAL_ITERABLE_HPP
