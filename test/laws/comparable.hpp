/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_COMPARABLE_HPP
#define BOOST_HANA_TEST_LAWS_COMPARABLE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename T, typename = when<true>>
    struct TestComparable : TestComparable<T, laws> {
        using TestComparable<T, laws>::TestComparable;
    };

    template <typename T>
    struct TestComparable<T, laws> {
        static_assert(_models<Comparable, T>{}, "");

        template <typename Xs>
        TestComparable(Xs xs) {
            foreach3(xs, [](auto a, auto b, auto c) {

                // reflexivity
                BOOST_HANA_CHECK(
                    hana::equal(a, a)
                );

                // symmetry
                BOOST_HANA_CHECK(
                    hana::equal(a, b) ^implies^ hana::equal(b, a)
                );

                // transitivity
                BOOST_HANA_CHECK(
                    hana::and_(hana::equal(a, b), hana::equal(b, c)) ^implies^ hana::equal(a, c)
                );

                // `not_equal` is the negation of `equal`
                BOOST_HANA_CHECK(
                    hana::not_equal(a, b) ^iff^ hana::not_(hana::equal(a, b))
                );

                // equal.to and not_equal.to
                BOOST_HANA_CHECK(
                    equal.to(a)(b) ^iff^ hana::equal(a, b)
                );

                BOOST_HANA_CHECK(
                    not_equal.to(a)(b) ^iff^ hana::not_equal(a, b)
                );

                // operators
                only_when_(bool_<has_operator<T, decltype(equal)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(
                        hana::equal(a, b) ^iff^ (_(a) == _(b))
                    );
                });

                only_when_(bool_<has_operator<T, decltype(not_equal)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(
                        hana::not_equal(a, b) ^iff^ (_(a) != _(b))
                    );
                });
            });
        }
    };

    template <typename C>
    struct TestComparable<C, when<_models<Constant, C>{}>>
        : TestComparable<C, laws>
    {
        template <typename Xs>
        TestComparable(Xs xs) : TestComparable<C, laws>{xs} {
            foreach2(xs, [](auto a, auto b) {

                BOOST_HANA_CHECK(
                    hana::value(hana::equal(a, b)) ^iff^
                        hana::equal(hana::value(a), hana::value(b))
                );

            });
        }
    };

    template <typename P>
    struct TestComparable<P, when<_models<Product, P>{}>> : TestComparable<P, laws> {
        template <typename Products>
        TestComparable(Products products) : TestComparable<P, laws>{products} {
            foreach2(products, [](auto x, auto y) {
                BOOST_HANA_CHECK(
                    hana::equal(x, y) ^iff^
                    hana::and_(
                        hana::equal(hana::first(x), hana::first(y)),
                        hana::equal(hana::second(x), hana::second(y))
                    )
                );
            });
        }
    };

    template <typename S>
    struct TestComparable<S, when<_models<Sequence, S>{}>> : TestComparable<S, laws> {
        template <int i>
        using x = _constant<i>;

        template <typename Xs>
        TestComparable(Xs xs) : TestComparable<S, laws>{xs} {
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // equal
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                list(),
                list()
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                list(x<0>{}),
                list()
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                list(),
                list(x<0>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                list(x<0>{}),
                list(x<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                list(x<0>{}, x<1>{}),
                list(x<0>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::equal(
                list(x<0>{}, x<1>{}),
                list(x<0>{}, x<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
                list(x<0>{}, x<1>{}, x<2>{}, x<3>{}),
                list(x<0>{}, x<1>{}, x<2>{}, x<4>{})
            )));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_COMPARABLE_HPP
