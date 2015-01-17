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

#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    template <>
    struct models_impl<Applicative> {
        template <typename A, typename Context>
        static constexpr bool apply =
            models<Functor, A, Context> &&
            is_implemented<lift_impl<A>, Context> &&
            is_implemented<ap_impl<A>, Context>
        ;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_HPP
