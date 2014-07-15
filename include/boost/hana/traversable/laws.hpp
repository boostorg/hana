/*!
@file
Defines `boost::hana::Traversable::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_LAWS_HPP
#define BOOST_HANA_TRAVERSABLE_LAWS_HPP

#include <boost/hana/traversable/traversable.hpp>

#include <boost/hana/bool.hpp>


namespace boost { namespace hana {
    //! @todo Implement this.
    struct Traversable::laws {
        static constexpr auto check() {
            return true_;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_LAWS_HPP
