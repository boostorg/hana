/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_ENUMERABLE_HPP
#define BOOST_HANA_TEST_LAWS_ENUMERABLE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/concept/enumerable.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename E, typename = when<true>>
    struct TestEnumerable : TestEnumerable<E, laws> {
        using TestEnumerable<E, laws>::TestEnumerable;
    };

    template <typename E>
    struct TestEnumerable<E, laws> {
        template <typename Xs>
        TestEnumerable(Xs xs) {
            hana::for_each(xs, [](auto n) {
                static_assert(_models<Enumerable, decltype(n)>{}, "");

                BOOST_HANA_CHECK(hana::equal(
                    hana::succ(hana::pred(n)),
                    n
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::pred(hana::succ(n)),
                    n
                ));

            });
        }
    };

    template <typename C>
    struct TestEnumerable<C, when<_models<Constant, C>{}()>>
        : TestEnumerable<C, laws>
    {
        template <typename Xs>
        TestEnumerable(Xs xs) : TestEnumerable<C, laws>{xs} {
            foreach(xs, [](auto c) {

                BOOST_HANA_CHECK(hana::equal(
                    hana::succ(hana::value(c)),
                    hana::value(hana::succ(c))
                ));

                BOOST_HANA_CHECK(hana::equal(
                    hana::pred(hana::value(c)),
                    hana::value(hana::pred(c))
                ));

            });
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_ENUMERABLE_HPP
