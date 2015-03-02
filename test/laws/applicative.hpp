/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_APPLICATIVE_HPP
#define BOOST_HANA_TEST_LAWS_APPLICATIVE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/applicative.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename F, typename = when<true>>
    struct TestApplicative : TestApplicative<F, laws> {
        using TestApplicative<F, laws>::TestApplicative;
    };

    template <typename F>
    struct TestApplicative<F, laws> {
        static_assert(models<Applicative(F)>{}, "");
        //! @todo Write Applicative laws
    };

    template <typename S>
    struct TestApplicative<S, when<models<Sequence(S)>{}>> : TestApplicative<S, laws> {
        template <int i>
        using eq = _constant<i>;

        TestApplicative() : TestApplicative<S, laws>{} {
            _injection<0> f{};
            _injection<1> g{};
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // ap
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(), list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(), list(eq<0>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(), list(eq<0>{}, eq<1>{})),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(), list(eq<0>{}, eq<1>{}, eq<2>{})),
                list()
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f), list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f), list(eq<0>{})),
                list(f(eq<0>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f), list(eq<0>{}, eq<1>{})),
                list(f(eq<0>{}), f(eq<1>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f), list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(f(eq<0>{}), f(eq<1>{}), f(eq<2>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f, g), list()),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f, g), list(eq<0>{})),
                list(f(eq<0>{}), g(eq<0>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f, g), list(eq<0>{}, eq<1>{})),
                list(f(eq<0>{}), f(eq<1>{}), g(eq<0>{}), g(eq<1>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                hana::ap(list(f, g), list(eq<0>{}, eq<1>{}, eq<2>{})),
                list(f(eq<0>{}), f(eq<1>{}), f(eq<2>{}), g(eq<0>{}), g(eq<1>{}), g(eq<2>{}))
            ));

            //////////////////////////////////////////////////////////////////
            // lift
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                lift<S>(eq<0>{}),
                list(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                lift<S>(eq<1>{}),
                list(eq<1>{})
            ));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_APPLICATIVE_HPP
