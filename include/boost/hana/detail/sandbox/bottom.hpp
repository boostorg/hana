/*!
@file
Defines `boost::hana::Bottom`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_BOTTOM_HPP
#define BOOST_HANA_BOTTOM_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>


namespace boost { namespace hana {
    //! @ingroup datatypes
    //! The `Bottom` data type represents statically type-erased data.
    struct Bottom { };

    BOOST_HANA_CONSTEXPR_LAMBDA auto bottom = [](auto x) {
        return detail::wrap<Bottom>(x);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_BOTTOM_HPP
