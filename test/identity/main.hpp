/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/identity.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>
#include <laws/traversable.hpp>
#include <test/cnumeric.hpp>
using namespace boost::hana;


namespace boost { namespace hana {
    template <>
    struct operators::of<test::Identity>
        : operators::of<Monad>
    { };
}}


int main() {
    using test::ct_eq;
    using test::cnumeric;
    test::_injection<0> f{};

    // Functor
    {
        auto functor = test::identity;
        // adjust_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                adjust_if(functor(ct_eq<0>{}), always(cnumeric<bool, true>), f),
                functor(f(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                adjust_if(functor(ct_eq<0>{}), always(cnumeric<bool, false>), f),
                functor(ct_eq<0>{})
            ));
        }

        // fill
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                fill(functor(ct_eq<0>{}), ct_eq<1>{}),
                functor(ct_eq<1>{})
            ));
        }

        // transform
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(functor(ct_eq<0>{}), f),
                functor(f(ct_eq<0>{}))
            ));
        }

        // replace_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                replace_if(functor(ct_eq<0>{}), always(cnumeric<bool, true>), ct_eq<1>{}),
                functor(ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                replace_if(functor(ct_eq<0>{}), always(cnumeric<bool, false>), ct_eq<1>{}),
                functor(ct_eq<0>{})
            ));
        }
    }

    // Applicative
    {
        auto a = test::identity;
        using A = test::Identity;

        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(ct_eq<0>{})),
                a(f(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(ct_eq<0>{}), a(ct_eq<1>{})),
                a(f(ct_eq<0>{}, ct_eq<1>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(ct_eq<0>{}), a(ct_eq<1>{}), a(ct_eq<2>{})),
                a(f(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(ct_eq<0>{}), a(ct_eq<1>{}), a(ct_eq<2>{}), a(ct_eq<3>{})),
                a(f(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<A>(ct_eq<0>{}),
                a(ct_eq<0>{})
            ));
        }
    }

    // Monad
    {
        auto monad = test::identity;
        using M = test::Identity;
        auto f = compose(monad, test::_injection<0>{});
        auto g = compose(monad, test::_injection<1>{});

        // chain
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                chain(monad(ct_eq<1>{}), f),
                f(ct_eq<1>{})
            ));
        }

        // tap
        {
            bool executed = false;
            auto exec = [&](auto) { executed = true; };
            BOOST_HANA_CONSTANT_CHECK(equal(
                chain(monad(ct_eq<0>{}), tap<M>(exec)),
                monad(ct_eq<0>{})
            ));
            BOOST_HANA_RUNTIME_CHECK(executed);
        }

        // then
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(equal(
                then(monad(invalid{}), monad(ct_eq<0>{})),
                monad(ct_eq<0>{})
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;

            BOOST_HANA_CONSTANT_CHECK(equal(
                monad(ct_eq<1>{}) | f,
                chain(monad(ct_eq<1>{}), f)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                monad(ct_eq<1>{}) | f | g,
                chain(chain(monad(ct_eq<1>{}), f), g)
            ));
        }
    }

    // Traversable
    {
        // traverse
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                traverse<test::Identity>(test::identity(ct_eq<0>{}), compose(test::identity, f)),
                test::identity(test::identity(f(ct_eq<0>{})))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                traverse<Tuple>(test::identity(ct_eq<0>{}), compose(make<Tuple>, f)),
                make<Tuple>(test::identity(f(ct_eq<0>{})))
            ));
        }

        // sequence
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                sequence<test::Identity>(test::identity(test::identity(ct_eq<0>{}))),
                test::identity(test::identity(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sequence<Tuple>(test::identity(make<Tuple>(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}))),
                make<Tuple>(test::identity(ct_eq<0>{}), test::identity(ct_eq<1>{}), test::identity(ct_eq<2>{}))
            ));
        }
    }
}
