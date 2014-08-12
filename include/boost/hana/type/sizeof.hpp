/*!
@file
Defines `boost::hana::sizeof_`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_SIZEOF_HPP
#define BOOST_HANA_TYPE_SIZEOF_HPP

#include <boost/hana/integral.hpp>
#include <boost/hana/type/type.hpp>


namespace boost { namespace hana { namespace type_detail {
    template <typename T>
    constexpr auto sizeof_::operator()(T) const
    { return size_t<sizeof(typename T::type)>; }
}}}

#endif // !BOOST_HANA_TYPE_SIZEOF_HPP
