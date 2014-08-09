/*!
@file
Defines `boost::hana::Ring::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RING_DETAIL_LAWS_HPP
#define BOOST_HANA_RING_DETAIL_LAWS_HPP

#include <boost/hana/ring/ring.hpp>

#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Ring::laws {
        template <typename Rings>
        static constexpr auto check(Rings rs) {
            return all(rs, [=](auto x) {
                return all(rs, [=](auto y) {
                    return all(rs, [=](auto z) {
                        using R = datatype_t<decltype(x)>;
                        return and_(
                            equal(mult(x, mult(y, z)), mult(mult(x, y), z)),
                            equal(mult(one<R>, x), x),
                            equal(mult(x, one<R>), x),
                            equal(mult(x, plus(y, z)), plus(mult(x, y), mult(x, z)))
                        );
                    });
                });
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_DETAIL_LAWS_HPP
