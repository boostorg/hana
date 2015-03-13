/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_FUNCTOR_HPP
#define BOOST_HANA_TEST_TEST_AUTO_FUNCTOR_HPP

#include <boost/hana/functor.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/logical.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>


namespace boost { namespace hana { namespace test {
    template <typename F>
    auto laws<Functor, F> = [] {
        static_assert(_models<Functor, F>{}, "");

        auto f = injection([]{});
        auto g = injection([]{});
        auto v = injection([]{})();
        auto pred = always(true_);

        for_each(objects<F>, [=](auto xs) {
            BOOST_HANA_CHECK(
                equal(transform(xs, id), xs)
            );

            BOOST_HANA_CHECK(equal(
                transform(xs, compose(f, g)),
                transform(transform(xs, g), f)
            ));

            BOOST_HANA_CHECK(equal(
                adjust_if(xs, pred, f),
                transform(xs, [=](auto x) {
                    return eval_if(pred(x),
                        [=](auto _) { return _(f)(x); },
                        [=](auto) { return x; }
                    );
                })
            ));

            BOOST_HANA_CHECK(equal(
                replace(xs, pred, v),
                adjust_if(xs, pred, always(v))
            ));

            BOOST_HANA_CHECK(equal(
                fill(xs, v),
                replace(xs, always(true_), v)
            ));
        });
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_FUNCTOR_HPP
