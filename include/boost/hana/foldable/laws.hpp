/*!
@file
Defines `boost::hana::Foldable::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_LAWS_HPP
#define BOOST_HANA_FOLDABLE_LAWS_HPP

#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/bool.hpp>


namespace boost { namespace hana {
    //! @todo Implement this.
    struct Foldable::laws {
        static constexpr auto check()
        { return true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_LAWS_HPP
