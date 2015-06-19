/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>
#include <laws/orderable.hpp>
#include <test/identity.hpp>

#include <type_traits>
using namespace boost::hana;


using test::ct_eq;
using test::ct_ord;
struct undefined { };

int main() {
    auto eqs = make<Tuple>(
        right(ct_eq<0>{}), left(ct_eq<0>{}),
        right(ct_eq<2>{}), left(ct_eq<2>{}),
        right(ct_eq<3>{}), left(ct_eq<3>{})
    );
    (void)eqs;

    auto big_eqs = make<Tuple>(
        right(ct_eq<0>{}), left(ct_eq<0>{}),
        right(ct_eq<1>{}), left(ct_eq<1>{}),
        right(ct_eq<2>{}), left(ct_eq<2>{}),
        right(ct_eq<3>{}), left(ct_eq<3>{})
    );
    (void)big_eqs;

    auto eq_keys = make<Tuple>(ct_eq<0>{}, ct_eq<2>{});
    (void)eq_keys;

    auto nested_eqs = make<Tuple>(
        right(right(ct_eq<2>{})), left(right(ct_eq<2>{})),
        right(right(ct_eq<3>{})), left(right(ct_eq<3>{})),

        right(left(ct_eq<0>{})), left(left(ct_eq<0>{})),
        right(left(ct_eq<1>{})), left(left(ct_eq<1>{}))
    );
    (void)nested_eqs;

    auto ords = make<Tuple>(
        right(ct_ord<0>{}), left(ct_ord<0>{}),
        right(ct_ord<2>{}), left(ct_ord<2>{}),
        right(ct_ord<3>{}), left(ct_ord<3>{})
    );
    (void)ords;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Interface
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};
        test::_injection<0> g{};

        // left
        {
            auto e = left(undefined{});
            static_assert(std::is_same<
                datatype_t<decltype(e)>, Either
            >::value, "");
        }

        // right
        {
            auto e = right(undefined{});
            static_assert(std::is_same<
                datatype_t<decltype(e)>, Either
            >::value, "");
        }

        // either
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                either(f, g, left(ct_eq<0>{})),
                f(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                either(f, g, right(ct_eq<0>{})),
                g(ct_eq<0>{})
            ));
        }

        // Make sure we do not instantiate rogue contructors when trying
        // to copy an Either.
        {
            {
                auto expr = right(test::trap_construct{});
                auto implicit_copy = expr;          (void)implicit_copy;
                decltype(expr) explicit_copy(expr); (void)explicit_copy;
            }

            {
                auto expr = left(test::trap_construct{});
                auto implicit_copy = expr;          (void)implicit_copy;
                decltype(expr) explicit_copy(expr); (void)explicit_copy;
            }
        }

        // implicit and explicit construction of left and right
        {
            _right<int> r1{1};       (void)r1;
            _right<int> r2 = {1};    (void)r2;

            _left<int> l1{1};       (void)l1;
            _left<int> l2 = {1};    (void)l2;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        {
            static_assert(right(1) == right(1), "");
            BOOST_HANA_CONSTANT_CHECK(left(ct_eq<0>{}) == left(ct_eq<0>{}));
            BOOST_HANA_CONSTANT_CHECK(left(ct_eq<1>{}) != left(ct_eq<0>{}));
            BOOST_HANA_CONSTANT_CHECK(right(ct_eq<1>{}) != right(ct_eq<0>{}));
            BOOST_HANA_CONSTANT_CHECK(right(ct_eq<0>{}) == right(ct_eq<0>{}));
            BOOST_HANA_CONSTANT_CHECK(left(ct_eq<0>{}) != right(ct_eq<0>{}));
        }

        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                left(ct_eq<0>{}),
                left(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                left(ct_eq<0>{}),
                left(ct_eq<1>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                left(undefined{}),
                right(undefined{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                right(undefined{}),
                left(undefined{})
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                right(ct_eq<0>{}),
                right(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                right(ct_eq<0>{}),
                right(ct_eq<1>{})
            )));
        }

        // laws
        test::TestComparable<Either>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(left(ct_ord<0>{}) < left(ct_ord<1>{}));
            BOOST_HANA_CONSTANT_CHECK(left(ct_ord<2>{}) >= left(ct_ord<1>{}));
            BOOST_HANA_CONSTANT_CHECK(left(ct_ord<2>{}) <= left(ct_ord<2>{}));
            BOOST_HANA_CONSTANT_CHECK(left(ct_ord<3>{}) > left(ct_ord<2>{}));

            BOOST_HANA_CONSTANT_CHECK(right(ct_ord<0>{}) < right(ct_ord<1>{}));
            BOOST_HANA_CONSTANT_CHECK(right(ct_ord<2>{}) >= right(ct_ord<1>{}));
            BOOST_HANA_CONSTANT_CHECK(right(ct_ord<2>{}) <= right(ct_ord<2>{}));
            BOOST_HANA_CONSTANT_CHECK(right(ct_ord<3>{}) > right(ct_ord<2>{}));
        }

        // less
        {
            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<0>{}),
                left(ct_ord<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                left(ct_ord<0>{}),
                left(ct_ord<0>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                left(ct_ord<1>{}),
                left(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(less(
                right(ct_ord<0>{}),
                right(ct_ord<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                right(ct_ord<0>{}),
                right(ct_ord<0>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                right(ct_ord<1>{}),
                right(ct_ord<0>{})
            )));

            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<0>{}),
                right(ct_ord<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<0>{}),
                right(ct_ord<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(less(
                left(ct_ord<1>{}),
                right(ct_ord<0>{})
            ));
        }

        // laws
        test::TestOrderable<Either>{ords};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(left(ct_eq<0>{}), f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(right(ct_eq<0>{}), f),
                f(ct_eq<0>{})
            ));
        }

        // laws
        test::TestFoldable<Either>{big_eqs};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        // transform
        {
            test::_injection<0> f{};
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(left(ct_eq<0>{}), undefined{}),
                left(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(right(ct_eq<0>{}), f),
                right(f(ct_eq<0>{}))
            ));
        }

        // laws
        test::TestFunctor<Either>{big_eqs, eq_keys};
    }

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(left(ct_eq<0>{}), left(undefined{})),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(left(ct_eq<0>{}), right(undefined{})),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(right(undefined{}), left(ct_eq<0>{})),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(right(f), right(ct_eq<0>{})),
                right(f(ct_eq<0>{}))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<Either>(ct_eq<0>{}),
                right(ct_eq<0>{})
            ));
        }

        // laws
        test::TestApplicative<Either>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        {
            test::_injection<0> f;
            BOOST_HANA_CONSTANT_CHECK(equal(
                left(ct_eq<0>{}) | f,
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                right(ct_eq<0>{}) | f,
                f(ct_eq<0>{})
            ));
        }

        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(left(left(ct_eq<0>{}))),
                left(left(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(left(right(ct_eq<0>{}))),
                left(right(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(right(left(ct_eq<0>{}))),
                left(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(right(right(ct_eq<0>{}))),
                right(ct_eq<0>{})
            ));
        }

        // laws
        test::TestMonad<Either>{big_eqs, nested_eqs};
    }
#endif
}
