/*!
@file
Defines `boost::hana::Orderable::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_DETAIL_LAWS_HPP
#define BOOST_HANA_ORDERABLE_DETAIL_LAWS_HPP

#include <boost/hana/orderable/orderable.hpp>

#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Orderable::laws {
        template <typename Orderables>
        static constexpr auto check(Orderables orderables) {
            auto implies = [](auto p, auto q) { return or_(not_(p), q); };
            auto incomparable = [](auto x, auto y) {
                return not_(or_(less(x, y), less(y, x)));
            };
            return all(orderables, [=](auto a) {
                return all(orderables, [=](auto b) {
                    return all(orderables, [=](auto c) {
                        return and_(
                            not(less(a, a)),
                            implies(less(a, b), not_(less(b, a))),
                            implies(and_(less(a, b), less(b, c)), less(a, c)),
                            implies(
                                and_(incomparable(a, b), incomparable(b, c)),
                                incomparable(a, c)
                            )
                        );
                    });
                });
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_DETAIL_LAWS_HPP
