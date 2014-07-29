/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_RECORD_UDT_HPP
#define BOOST_HANA_TEST_RECORD_UDT_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/record/mcd.hpp>
#include <boost/hana/type.hpp>


struct udt {
    int member1;
    char member2;
};

constexpr auto member1 = boost::hana::decltype_(&udt::member1);
constexpr auto member2 = boost::hana::decltype_(&udt::member2);

namespace boost { namespace hana {
    template <>
    struct Record::instance<udt> : Record::mcd {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto members_impl() {
            return list(
                pair(member1, [](auto u) { return u.member1; }),
                pair(member2, [](auto u) { return u.member2; })
            );
        }
    };
}}

#endif // !BOOST_HANA_TEST_RECORD_UDT_HPP
