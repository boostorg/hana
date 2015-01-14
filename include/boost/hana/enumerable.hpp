/*!
@file
Defines `boost::hana::Enumerable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ENUMERABLE_HPP
#define BOOST_HANA_ENUMERABLE_HPP

#include <boost/hana/fwd/enumerable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct succ_impl<T, when_valid<
        decltype(++detail::std::declval<T&>())
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return ++x; }
    };

    template <typename T>
    struct pred_impl<T, when_valid<
        decltype(--detail::std::declval<T&>())
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return --x; }
    };

    template <typename T>
    constexpr auto is_a<Enumerable, T> = bool_<
        is_implemented<succ_impl<T>> &&
        is_implemented<pred_impl<T>>
    >;
}}

#endif // !BOOST_HANA_ENUMERABLE_HPP
