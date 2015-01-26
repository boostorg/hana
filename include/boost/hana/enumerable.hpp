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

#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // succ
    //////////////////////////////////////////////////////////////////////////
    template <typename E, typename>
    struct succ_impl : succ_impl<E, when<true>> { };

    template <typename E, bool condition>
    struct succ_impl<E, when<condition>> {
        static_assert(wrong<succ_impl<E>>{},
        "no definition of boost::hana::succ for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // pred
    //////////////////////////////////////////////////////////////////////////
    template <typename E, typename>
    struct pred_impl : pred_impl<E, when<true>> { };

    template <typename E, bool condition>
    struct pred_impl<E, when<condition>> {
        static_assert(wrong<pred_impl<E>>{},
        "no definition of boost::hana::pred for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for incrementable and decrementable data types
    //////////////////////////////////////////////////////////////////////////
    template <typename E>
    struct models<Enumerable(E), when_valid<
        decltype(++detail::std::declval<E&>()),
        decltype(--detail::std::declval<E&>())
    >>
        : detail::std::true_type
    { };

    template <typename E>
    struct succ_impl<E, when_valid<
        decltype(++detail::std::declval<E&>()),
        decltype(--detail::std::declval<E&>())
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return ++x; }
    };

    template <typename E>
    struct pred_impl<E, when_valid<
        decltype(++detail::std::declval<E&>()),
        decltype(--detail::std::declval<E&>())
    >> {
        template <typename X>
        static constexpr auto apply(X /* by value */ x)
        { return --x; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ENUMERABLE_HPP
