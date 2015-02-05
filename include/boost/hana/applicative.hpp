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

#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition : `lift` and `ap`
    struct Applicative::mcd { };

    template <typename T>
    struct Applicative::instance<T, when<models<Applicative(T)>{}>>
        : Applicative::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_HPP
