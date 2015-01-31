/*!
@file
Forward declares `boost::hana::ext::boost::mpl::IntegralC`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_BOOST_MPL_INTEGRAL_C_HPP
#define BOOST_HANA_FWD_EXT_BOOST_MPL_INTEGRAL_C_HPP

namespace boost { namespace hana {
    namespace ext { namespace boost { namespace mpl {
        //! @ingroup group-datatypes
        //! Data type representing Boost.MPL IntegralConstants.
        template <typename T>
        struct IntegralC { using value_type = T; };
    }}}
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_BOOST_MPL_INTEGRAL_C_HPP
