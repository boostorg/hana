/*!
@file
Defines `boost::hana::Comparable::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_DETAIL_LAWS_HPP
#define BOOST_HANA_COMPARABLE_DETAIL_LAWS_HPP

#include <boost/hana/comparable/comparable.hpp>

#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Comparable::laws {
        template <typename ComparableObjects>
        static constexpr auto check(ComparableObjects objs) {
            auto implies = [](auto p, auto q) { return or_(not_(p), q); };
            return all(objs, [=](auto a) {
                return all(objs, [=](auto b) {
                    return all(objs, [=](auto c) {
                        return and_(
                            equal(a, a),
                            implies(equal(a, b), equal(b, a)),
                            implies(and_(equal(a, b), equal(b, c)), equal(a, c))
                        );
                    });
                });
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_DETAIL_LAWS_HPP
