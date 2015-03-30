/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_MONAD_HPP
#define BOOST_HANA_TEST_LAWS_MONAD_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/monad.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename M, typename = when<true>>
    struct TestMonad : TestMonad<M, laws> {
        using TestMonad<M, laws>::TestMonad;
    };

    template <typename M>
    struct TestMonad<M, laws> {
        static_assert(_models<Monad, M>{}, "");

        // Xs are Monads over something
        // XXs are Monads over Monads over something
        template <typename Xs, typename XXs>
        TestMonad(Xs xs, XXs xxs) {
            hana::for_each(xs, [](auto m) {
                auto f = hana::compose(lift<M>, test::_injection<0>{});
                auto g = hana::compose(lift<M>, test::_injection<1>{});
                auto h = hana::compose(lift<M>, test::_injection<2>{});
                auto x = test::ct_eq<0>{};

                // Laws formulated with mcompose:
                // left identity
                BOOST_HANA_CHECK(hana::equal(
                    hana::mcompose<M>(lift<M>, f)(x),
                    f(x)
                ));

                // right identity
                BOOST_HANA_CHECK(hana::equal(
                    hana::mcompose<M>(f, lift<M>)(x),
                    f(x)
                ));

                // associativity
                BOOST_HANA_CHECK(hana::equal(
                    hana::mcompose<M>(hana::mcompose<M>(f, g), h)(x),
                    hana::mcompose<M>(f, hana::mcompose<M>(g, h))(x)
                ));


                // Laws formulated with bind:
                BOOST_HANA_CHECK(hana::equal(
                    hana::bind(hana::lift<M>(x), f),
                    f(x)
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::bind(m, lift<M>),
                    m
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::bind(m, [f, g](auto x) {
                        return hana::bind(f(x), g);
                    }),
                    hana::bind(hana::bind(m, f), g)
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::transform(m, f),
                    hana::bind(m, hana::compose(lift<M>, f))
                ));


                // Consistency of the method definitions
                BOOST_HANA_CHECK(hana::equal(
                    hana::bind(m, f),
                    hana::flatten(hana::transform(m, f))
                ));

            });

            hana::for_each(xxs, [](auto mm) {
                BOOST_HANA_CHECK(hana::equal(
                    hana::flatten(mm),
                    hana::bind(mm, id)
                ));
            });
        }
    };

    template <typename S>
    struct TestMonad<S, when<_models<Sequence, S>{}>> : TestMonad<S, laws> {
        template <typename Xs, typename XXs>
        TestMonad(Xs xs, XXs xxs) : TestMonad<S, laws>{xs, xxs} {
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // flatten
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::flatten(list(list(), list())),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::flatten(list(list(ct_eq<0>{}), list())),
                list(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::flatten(list(list(), list(ct_eq<0>{}))),
                list(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::flatten(list(list(ct_eq<0>{}), list(ct_eq<1>{}))),
                list(ct_eq<0>{}, ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::flatten(list(
                    list(ct_eq<0>{}, ct_eq<1>{}),
                    list(),
                    list(ct_eq<2>{}, ct_eq<3>{}),
                    list(ct_eq<4>{})
                )),
                list(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{})
            ));

            // just make sure we don't double move; this happened in Tuple
            flatten(list(list(Tracked{1}, Tracked{2})));

            //////////////////////////////////////////////////////////////////
            // bind
            //////////////////////////////////////////////////////////////////
            {
                test::_injection<0> f{};
                auto g = [=](auto x) { return list(f(x)); };

                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    hana::bind(list(), g),
                    list()
                ));

                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    hana::bind(list(ct_eq<1>{}), g),
                    list(f(ct_eq<1>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    hana::bind(list(ct_eq<1>{}, ct_eq<2>{}), g),
                    list(f(ct_eq<1>{}), f(ct_eq<2>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    hana::bind(list(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}), g),
                    list(f(ct_eq<1>{}), f(ct_eq<2>{}), f(ct_eq<3>{}))
                ));

                BOOST_HANA_CONSTANT_CHECK(hana::equal(
                    hana::bind(list(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}), g),
                    list(f(ct_eq<1>{}), f(ct_eq<2>{}), f(ct_eq<3>{}), f(ct_eq<4>{}))
                ));
            }
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_MONAD_HPP
