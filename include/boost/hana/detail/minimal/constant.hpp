/*!
@file
Defines a minimal instance for `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_CONSTANT_HPP
#define BOOST_HANA_DETAIL_MINIMAL_CONSTANT_HPP

#include <boost/hana/constant/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename T, T v, typename mcd>
    struct constant_impl {
        static constexpr T value = v;
        using hana_datatype = mcd;
    };

    template <typename T, T v, typename mcd = Constant::mcd>
    constexpr constant_impl<T, v, mcd> constant{};
}} // end namespace detail::minimal

template <>
struct Constant::instance<Constant::mcd> : Constant::mcd {
    template <typename C>
    static constexpr auto value_impl(C const&)
    { return C::value; }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_CONSTANT_HPP
