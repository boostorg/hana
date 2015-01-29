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

#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    template <typename T>
    struct default_<sequence_impl<T>> {
        template <typename A, typename Trav>
        static constexpr auto apply(Trav traversable)
        { return traverse<A>(traversable, id); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_HPP
