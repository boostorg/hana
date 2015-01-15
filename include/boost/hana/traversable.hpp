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

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    template <typename T, typename _>
    struct sequence_impl<T, when<is_implemented<traverse_impl<T>, _>>, _> {
        template <typename A, typename Trav>
        static constexpr auto apply(Trav traversable)
        { return traverse<A>(traversable, id); }
    };

    template <typename T>
    constexpr auto is_a<Traversable, T> = bool_<
        is_a<Functor, T>() &&
        is_implemented<traverse_impl<T>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_HPP
