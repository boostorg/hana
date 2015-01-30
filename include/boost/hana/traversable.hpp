/*!
@file
Defines `boost::hana::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_HPP
#define BOOST_HANA_TRAVERSABLE_HPP

#include <boost/hana/fwd/traversable.hpp>

#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // sequence
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct sequence_impl : sequence_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct sequence_impl<T, when<condition>> {
        template <typename A, typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::traverse<A>(detail::std::forward<Xs>(xs), id); }
    };

    //////////////////////////////////////////////////////////////////////////
    // traverse
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct traverse_impl : traverse_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct traverse_impl<T, when<condition>> {
        static_assert(wrong<traverse_impl<T>>{},
        "no definition of boost::hana::traverse for the given data type");
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_HPP
