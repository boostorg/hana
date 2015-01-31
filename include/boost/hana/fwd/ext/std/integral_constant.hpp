/*!
@file
Forward declares `boost::hana::ext::std::IntegralConstant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_STD_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_FWD_EXT_STD_INTEGRAL_CONSTANT_HPP

namespace boost { namespace hana {
    namespace ext { namespace std {
        //! @ingroup group-datatypes
        //! Data type representing `std::integral_constant`s.
        template <typename T>
        struct IntegralConstant { using value_type = T; };
    }}
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_STD_INTEGRAL_CONSTANT_HPP
