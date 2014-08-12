/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_ITERABLE_HPP
#define BOOST_HANA_TEST_TEST_LAWS_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/iterable/iterable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/tuple.hpp>


template <typename ...Iterables>
constexpr auto Iterable_laws(Iterables ...xs) {
    using namespace boost::hana;
    auto iff = infix([](auto p, auto q) { return or_(not_(p), q); });
    return and_(
        and_(
            eval_if(is_empty(xs),
                always(true_),
                [=](auto _) {
                    return and_(
                        equal(head(_(xs)), head(to<Tuple>(_(xs)))),
                        equal(to<Tuple>(tail(_(xs))), tail(to<Tuple>(_(xs))))
                    );
                }
            ),
            is_empty(xs) ^iff^ is_empty(to<Tuple>(xs))
        )...
    );
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_ITERABLE_HPP
