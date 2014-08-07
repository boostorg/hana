/*!
@file
Defines `boost::hana::Monoid::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_LAWS_HPP
#define BOOST_HANA_MONOID_LAWS_HPP

#include <boost/hana/monoid/monoid.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Monoid::laws {
        template <typename MonoidObjects>
        static constexpr auto check(MonoidObjects objs) {
            return all(objs, [=](auto x) {
                return all(objs, [=](auto y) {
                    return all(objs, [=](auto z) {
                        using M = datatype_t<decltype(x)>;
                        return and_(
                            equal(plus(zero<M>, x), x),
                            equal(plus(x, zero<M>), x),
                            equal(plus(x, plus(y, z)), plus(plus(x, y), z))
                        );
                    });
                });
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_LAWS_HPP
