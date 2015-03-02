/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_TRAVERSABLE_HPP
#define BOOST_HANA_TEST_LAWS_TRAVERSABLE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/traversable.hpp>

#include <laws/base.hpp>
#include <test/identity.hpp>


namespace boost { namespace hana { namespace test {
    template <typename T, typename = when<true>>
    struct TestTraversable : TestTraversable<T, laws> {
        using TestTraversable<T, laws>::TestTraversable;
    };

    template <typename T>
    struct TestTraversable<T, laws> {
        static_assert(models<Traversable(T)>{}, "");


        TestTraversable() {
            //! @todo Write Traversable laws
        }
    };

    template <typename S>
    struct TestTraversable<S, when<models<Sequence(S)>{}>> : TestTraversable<S, laws> {
        template <int i>
        using x = _constant<i>;

        TestTraversable() : TestTraversable<S, laws>{} {
            constexpr auto list = make<S>;
            _injection<0> f{};
            auto a = identity;
            using A = Identity;

            //////////////////////////////////////////////////////////////////
            // traverse
            //////////////////////////////////////////////////////////////////
            auto af = hana::compose(a, f);
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                traverse<A>(list(), af),
                a(list())
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                traverse<A>(list(x<0>{}), af),
                a(list(f(x<0>{})))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                traverse<A>(list(x<0>{}, x<1>{}), af),
                a(list(f(x<0>{}), f(x<1>{})))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                traverse<A>(list(x<0>{}, x<1>{}, x<2>{}), af),
                a(list(f(x<0>{}), f(x<1>{}), f(x<2>{})))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                traverse<A>(list(x<0>{}, x<1>{}, x<2>{}, x<3>{}), af),
                a(list(f(x<0>{}), f(x<1>{}), f(x<2>{}), f(x<3>{})))
            ));

            //////////////////////////////////////////////////////////////////
            // sequence
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                sequence<A>(list()),
                a(list())
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                sequence<A>(list(a(x<0>{}))),
                a(list(x<0>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                sequence<A>(list(a(x<0>{}), a(x<1>{}))),
                a(list(x<0>{}, x<1>{}))
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                sequence<A>(list(a(x<0>{}), a(x<1>{}), a(x<2>{}))),
                a(list(x<0>{}, x<1>{}, x<2>{}))
            ));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_TRAVERSABLE_HPP
