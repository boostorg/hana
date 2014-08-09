/*!
@file
Defines `boost::hana::Constant::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_DETAIL_LAWS_HPP
#define BOOST_HANA_CONSTANT_DETAIL_LAWS_HPP

#include <boost/hana/constant/constant.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Constant::laws {
        template <typename Cs>
        static constexpr auto check(Cs constants) {
            return all(constants, [](auto c) {
                constexpr auto must_be_constexpr = value(c);
                (void)must_be_constexpr;
                return true_;
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_DETAIL_LAWS_HPP
