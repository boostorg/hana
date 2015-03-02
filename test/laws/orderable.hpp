/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_ORDERABLE_HPP
#define BOOST_HANA_TEST_LAWS_ORDERABLE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/orderable.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename T, typename = when<true>>
    struct TestOrderable : TestOrderable<T, laws> {
        using TestOrderable<T, laws>::TestOrderable;
    };

    template <typename T>
    struct TestOrderable<T, laws> {
        static_assert(models<Orderable(T)>{}, "");

        template <typename Xs>
        TestOrderable(Xs xs) {
            foreach3(xs, [](auto a, auto b, auto c) {

                // antisymmetry
                BOOST_HANA_CHECK(
                    hana::and_(hana::less_equal(a, b), hana::less_equal(b, a))
                        ^implies^ hana::equal(a, b)
                );

                // transitivity
                BOOST_HANA_CHECK(
                    hana::and_(hana::less_equal(a, b), hana::less_equal(b, c))
                        ^implies^ hana::less_equal(a, c)
                );

                // totality
                BOOST_HANA_CHECK(
                    hana::or_(hana::less_equal(a, b), hana::less_equal(b, a))
                );

                // other methods in terms of `less_equal`
                BOOST_HANA_CHECK(
                    hana::less(a, b) ^iff^ hana::not_(hana::less_equal(b, a))
                );

                BOOST_HANA_CHECK(
                    hana::greater(a, b) ^iff^ hana::less(b, a)
                );

                BOOST_HANA_CHECK(
                    hana::greater_equal(a, b) ^iff^ hana::not_(hana::less(a, b))
                );

                // less.than & al.
                BOOST_HANA_CHECK(less.than(a)(b) ^iff^ hana::less(b, a));
                BOOST_HANA_CHECK(greater.than(a)(b) ^iff^ hana::greater(b, a));
                BOOST_HANA_CHECK(less_equal.than(a)(b) ^iff^ hana::less_equal(b, a));
                BOOST_HANA_CHECK(greater_equal.than(a)(b) ^iff^ hana::greater_equal(b, a));

                // operators
                only_when_(bool_<has_operator<T, decltype(less)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(
                        hana::less(a, b) ^iff^ (_(a) < _(b))
                    );
                });

                only_when_(bool_<has_operator<T, decltype(less_equal)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(
                        hana::less_equal(a, b) ^iff^ (_(a) <= _(b))
                    );
                });

                only_when_(bool_<has_operator<T, decltype(greater_equal)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(
                        hana::greater_equal(a, b) ^iff^ (_(a) >= _(b))
                    );
                });

                only_when_(bool_<has_operator<T, decltype(greater)>{}>, [=](auto _) {
                    BOOST_HANA_CHECK(
                        hana::greater(a, b) ^iff^ (_(a) > _(b))
                    );
                });
            });
        }
    };

    template <typename C>
    struct TestOrderable<C, when<models<Constant(C)>{}>>
        : TestOrderable<C, laws>
    {
        template <typename Xs>
        TestOrderable(Xs xs) : TestOrderable<C, laws>{xs} {
            foreach2(xs, [](auto a, auto b) {

                BOOST_HANA_CHECK(
                    hana::value(hana::less(a, b)) ^iff^
                        hana::less(hana::value(a), hana::value(b))
                );

            });
        }
    };

    template <typename P>
    struct TestOrderable<P, when<models<Product(P)>{}>> : TestOrderable<P, laws> {
        template <typename Products>
        TestOrderable(Products products) : TestOrderable<P, laws>{products} {
            foreach2(products, [](auto x, auto y) {
                BOOST_HANA_CHECK(
                    hana::less(x, y) ^iff^
                    hana::or_(
                        hana::less(hana::first(x), hana::first(y)),
                        hana::and_(
                            hana::equal(hana::first(x), hana::first(y)),
                            hana::less(hana::second(x), hana::second(y))
                        )
                    )
                );
            });
        }
    };

    template <typename S>
    struct TestOrderable<S, when<models<Sequence(S)>{}>> : TestOrderable<S, laws> {
        template <int i>
        using ord = _constant<i>;

        struct invalid { };

        template <typename Xs>
        TestOrderable(Xs xs) : TestOrderable<S, laws>{xs} {
            constexpr auto list = make<S>;

            //////////////////////////////////////////////////////////////////
            // less
            //////////////////////////////////////////////////////////////////
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
                list(),
                list()
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::less(
                list(),
                list(invalid{})
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
                list(invalid{}),
                list()
            )));
            BOOST_HANA_CONSTANT_CHECK(hana::less(
                list(ord<0>{}),
                list(ord<7>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
                list(ord<1>{}),
                list(ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
                list(ord<0>{}, ord<1>{}, ord<8>{}),
                list(ord<0>{}, ord<1>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(hana::less(
                list(ord<0>{}, ord<0>{}, ord<8>{}),
                list(ord<0>{}, ord<1>{})
            ));
        }
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_LAWS_ORDERABLE_HPP
