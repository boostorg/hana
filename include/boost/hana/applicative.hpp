/*!
@file
Defines `boost::hana::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_HPP
#define BOOST_HANA_APPLICATIVE_HPP

#include <boost/hana/fwd/applicative.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    template <typename A>
    constexpr auto is_a<Applicative, A> = bool_<
        is_a<Functor, A>() &&
        is_implemented<lift_impl<A>> &&
        is_implemented<ap_impl<A>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_HPP
