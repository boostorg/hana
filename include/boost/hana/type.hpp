/*!
@file
Defines `boost::hana::Type` and `boost::hana::Metafunction`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/fwd/type.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    template <>
    struct equal_impl<Type, Type> {
        template <typename T, typename U>
        static constexpr auto apply(T, U)
        { return false_; }

        template <typename T>
        static constexpr auto apply(T, T)
        { return true_; }
    };

#ifndef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T>
    constexpr auto _sizeof::operator()(T) const
    { return size_t<sizeof(typename T::type)>; }
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
