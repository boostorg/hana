/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAWS_FUNCTOR_HPP
#define BOOST_HANA_TEST_TEST_LAWS_FUNCTOR_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/logical/logical.hpp>

#include <test/injection.hpp>


template <typename ...Functors>
constexpr auto Functor_laws(Functors ...xs) {
    using namespace boost::hana;
    auto f = test::injection([]{});
    auto g = test::injection([]{});
    auto v = test::injection([]{})();
    auto pred = [](auto x) { return true_; };

    return and_(
        and_(
            equal(fmap(xs, id), xs),
            equal(
                fmap(xs, compose(f, g)),
                fmap(fmap(xs, g), f)
            ),

            equal(
                adjust(xs, pred, f),
                fmap(xs, [=](auto x) {
                    return eval_if(pred(x),
                        [=](auto _) { return _(f)(x); },
                        [=](auto) { return x; }
                    );
                })
            ),

            equal(
                replace(xs, pred, v),
                adjust(xs, pred, always(v))
            ),

            equal(
                fill(xs, v),
                replace(xs, always(true_), v)
            )
        )...
    );
}

#endif // !BOOST_HANA_TEST_TEST_LAWS_FUNCTOR_HPP
