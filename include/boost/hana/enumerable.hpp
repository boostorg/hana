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
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `succ` and `pred`
    struct Enumerable::mcd { };

    //! Instance of `Enumerable` for objects of foreign numeric types.
    //!
    //! Any foreign object that can be incremented using `operator++` and
    //! decremented using `operator--` is `Enumerable` using those operations
    //! for `succ` and `pred` respectively.
    template <typename T>
    struct Enumerable::instance<T, when_valid<
        decltype(++detail::std::declval<T&>()),
        decltype(--detail::std::declval<T&>())
    >>
        : Enumerable::mcd
    {
        template <typename X>
        static constexpr auto succ_impl(X /* by value */ x)
        { return ++x; }

        template <typename X>
        static constexpr auto pred_impl(X /* by value */ x)
        { return --x; }
    };

    template <typename T>
    struct Enumerable::instance<T, when<models<Enumerable(T)>{}>>
        : Enumerable::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ENUMERABLE_HPP
