/*!
@file
Defines `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_HPP
#define BOOST_HANA_CONSTANT_HPP

#include <boost/hana/fwd/constant.hpp>

#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `value`
    struct Constant::mcd { };

    template <typename T>
    struct Constant::instance<T, when<models<Constant(T)>{}>>
        : Constant::mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_HPP
