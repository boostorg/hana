/*!
@file
Defines `boost::hana::Group::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_LAWS_HPP
#define BOOST_HANA_GROUP_LAWS_HPP

#include <boost/hana/group/group.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Group::laws {
        template <typename GroupObjects>
        static constexpr auto check(GroupObjects objs) {
            return all(objs, [=](auto x) {
                using G = datatype_t<decltype(x)>;
                return and_(
                    equal(plus(x, negate(x)), zero<G>),
                    equal(plus(negate(x), x), zero<G>)
                );
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_LAWS_HPP
