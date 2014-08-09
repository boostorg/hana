/*!
@file
Defines the `boost::hana::Constant` instance for `boost::hana::detail::CNumber`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CNUMBER_CONSTANT_HPP
#define BOOST_HANA_DETAIL_CNUMBER_CONSTANT_HPP

#include <boost/hana/detail/cnumber/cnumber.hpp>

#include <boost/hana/constant/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd>
    struct Constant::instance<detail::CNumber<Mcd>>
        : Constant::mcd
    {
        template <typename X>
        static constexpr auto value_impl(X const&) {
            return X::value;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_CNUMBER_CONSTANT_HPP
