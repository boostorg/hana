/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/tuple.hpp>

#include <boost/hana/functional/always.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/functor.hpp>
#include <laws/iterable.hpp>
#include <laws/monad.hpp>
#include <laws/monad_plus.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>
#include <laws/sequence.hpp>

#include <string>
#include <type_traits>
#include <utility>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

template <int i>
using ord = test::ct_ord<i>;

struct x0; struct x1; struct x2; struct x3; struct x4;

template <typename ...>
struct F { struct type; };

// a non-movable, non-copyable type
struct ref_only {
    test::Tracked _track{1};
    ref_only() = default;
    ref_only(ref_only const&) = delete;
    ref_only(ref_only&&) = delete;
};


int main() {
    auto eq_tuples = make<tuple_tag>(
          make<tuple_tag>()
        , make<tuple_tag>(eq<0>{})
        , make<tuple_tag>(eq<0>{}, eq<1>{})
        , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{})
        , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
        , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
        , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}, eq<5>{})
    );
    (void)eq_tuples;

    auto eq_values = make<tuple_tag>(eq<0>{}, eq<2>{}, eq<4>{});
    (void)eq_values;

    auto predicates = make<tuple_tag>(
        equal.to(eq<0>{}), equal.to(eq<2>{}), equal.to(eq<4>{}),
        always(true_c), always(false_c)
    );
    (void)predicates;

    auto ord_tuples = make<tuple_tag>(
          make<tuple_tag>()
        , make<tuple_tag>(ord<0>{})
        , make<tuple_tag>(ord<0>{}, ord<1>{})
        , make<tuple_tag>(ord<0>{}, ord<1>{}, ord<2>{})
        , make<tuple_tag>(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{})
        , make<tuple_tag>(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{}, ord<4>{})
    );
    (void)ord_tuples;

    auto nested_eqs = make<tuple_tag>(
          make<tuple_tag>()
        , make<tuple_tag>(make<tuple_tag>(eq<0>{}))
        , make<tuple_tag>(make<tuple_tag>(eq<0>{}), make<tuple_tag>(eq<1>{}, eq<2>{}))
        , make<tuple_tag>(make<tuple_tag>(eq<0>{}),
                      make<tuple_tag>(eq<1>{}, eq<2>{}),
                      make<tuple_tag>(eq<3>{}, eq<4>{}))
    );
    (void)nested_eqs;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Move-only friendliness and reference semantics
    //////////////////////////////////////////////////////////////////////////
    {
        {
            auto xs = make<tuple_tag>(test::move_only{});
            auto by_val = [](auto) { };

            by_val(std::move(xs));
            by_val(front(std::move(xs)));
            by_val(at_c<0>(std::move(xs)));
            by_val(back(std::move(xs)));
        }

        {
            auto const& xs = make<tuple_tag>(test::move_only{});
            auto by_const_ref = [](auto const&) { };

            by_const_ref(xs);
            by_const_ref(front(xs));
            by_const_ref(at_c<0>(xs));
            by_const_ref(back(xs));
        }

        {
            auto xs = make<tuple_tag>(test::move_only{});
            auto by_ref = [](auto&) { };

            by_ref(xs);
            by_ref(front(xs));
            by_ref(at_c<0>(xs));
            by_ref(back(xs));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // default-constructibility
    //////////////////////////////////////////////////////////////////////////
    {
        tuple<> z1{}; (void)z1;
        tuple<int> z2{}; (void)z2;
        tuple<int, char> z3{}; (void)z3;
        tuple<int, char, float> z4{}; (void)z4;

        using Types = decltype(tuple_t<x0, x1>);
        Types default_{}; (void)default_;
    }

    //////////////////////////////////////////////////////////////////////////
    // In some cases where a type has a constructor that is way too
    // general, copying a unary tuple holding an object of that type
    // could trigger the instantiation of that constructor. If that
    // constructor was ill-formed, the compilation could fail. We
    // make sure this does not happen.
    //////////////////////////////////////////////////////////////////////////
    {
        auto expr = make<tuple_tag>(test::trap_construct{});
        auto implicit_copy = expr;          (void)implicit_copy;
        decltype(expr) explicit_copy(expr); (void)explicit_copy;
    }

    //////////////////////////////////////////////////////////////////////////
    // This used to trigger an ICE on Clang
    //////////////////////////////////////////////////////////////////////////
    {
        struct Car { std::string name; };
        auto stuff = make<tuple_tag>(Car{}, Car{}, Car{});
        any_of(stuff, [](auto&&) { return true; });
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal

        // tuple_t
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<>, tuple_t<>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<>, tuple_t<x0>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0>, tuple_t<x0>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0, x1>, tuple_t<x0>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<x0, x1>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1>, tuple_t<x0, x1>));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1, x2>, tuple_t<x0, x1, x2>));

        // tuple_c
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int>, tuple_c<int>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_c<int, 0>, tuple_c<int>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0>, tuple_c<int, 0>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_c<int, 0, 1>, tuple_c<int, 0>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0, 1>, tuple_c<int, 0, 1>));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0, 1, 2>, tuple_c<int, 0, 1, 2>));

        test::TestComparable<tuple_tag>{eq_tuples};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestOrderable<tuple_tag>{ord_tuples};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};

            // tuple
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<tuple_tag>(), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<tuple_tag>(eq<0>{}), f),
                f(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<tuple_tag>(eq<0>{}, eq<1>{}), f),
                f(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{}), f),
                f(eq<0>{}, eq<1>{}, eq<2>{})
            ));

            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<>, f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0>, f),
                f(type_c<x0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1>, f),
                f(type_c<x0>, type_c<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2>, f),
                f(type_c<x0>, type_c<x1>, type_c<x2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2, x3>, f),
                f(type_c<x0>, type_c<x1>, type_c<x2>, type_c<x3>)
            ));

            // tuple_t with metafunction
            auto g = metafunction<F>;
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<>, g),
                g()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0>, g),
                g(type_c<x0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1>, g),
                g(type_c<x0>, type_c<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2>, g),
                g(type_c<x0>, type_c<x1>, type_c<x2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2, x3>, g),
                g(type_c<x0>, type_c<x1>, type_c<x2>, type_c<x3>)
            ));
        }

        // fold_left with tuple_t and initial state
        {
            auto f = metafunction<F>;
            auto s = type_c<struct initial_state>;
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<>, s, f),
                s
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0>, s, f),
                f(s, type_c<x0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0, x1>, s, f),
                f(f(s, type_c<x0>), type_c<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0, x1, x2>, s, f),
                f(f(f(s, type_c<x0>), type_c<x1>), type_c<x2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0, x1, x2, x3>, s, f),
                f(f(f(f(s, type_c<x0>), type_c<x1>), type_c<x2>), type_c<x3>)
            ));
        }

        // fold_left with tuple_t and no initial state
        {
            auto f = metafunction<F>;
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0>, f),
                type_c<x0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0, x1>, f),
                f(type_c<x0>, type_c<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0, x1, x2>, f),
                f(f(type_c<x0>, type_c<x1>), type_c<x2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_left(tuple_t<x0, x1, x2, x3>, f),
                f(f(f(type_c<x0>, type_c<x1>), type_c<x2>), type_c<x3>)
            ));
        }

        // fold_right with tuple_t and an initial state
        {
            auto f = metafunction<F>;
            auto s = type_c<struct initial_state>;
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<>, s, f),
                s
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0>, s, f),
                f(type_c<x0>, s)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0, x1>, s, f),
                f(type_c<x0>, f(type_c<x1>, s))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0, x1, x2>, s, f),
                f(type_c<x0>, f(type_c<x1>, f(type_c<x2>, s)))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0, x1, x2, x3>, s, f),
                f(type_c<x0>, f(type_c<x1>, f(type_c<x2>, f(type_c<x3>, s))))
            ));
        }

        // fold_right with tuple_t and no initial state
        {
            auto f = metafunction<F>;
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0>, f),
                type_c<x0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0, x1>, f),
                f(type_c<x0>, type_c<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0, x1, x2>, f),
                f(type_c<x0>, f(type_c<x1>, type_c<x2>))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fold_right(tuple_t<x0, x1, x2, x3>, f),
                f(type_c<x0>, f(type_c<x1>, f(type_c<x2>, type_c<x3>)))
            ));
        }

        test::TestFoldable<tuple_tag>{eq_tuples};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // front
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(front(tuple_t<x0>), type_c<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(front(tuple_t<x0, x1>), type_c<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(front(tuple_t<x0, x1, x2>), type_c<x0>));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(equal(front(tuple_c<int, 0>), int_c<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(front(tuple_c<int, 0, 1>), int_c<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(front(tuple_c<int, 0, 1, 2>), int_c<0>));
        }

        // is_empty
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0, x1>)));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_c<int>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1, 2>)));
        }

        // tail
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_t<x0>),
                tuple_t<>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_t<x0, x1>),
                tuple_t<x1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_t<x0, x1, x2>),
                tuple_t<x1, x2>
            ));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0>),
                tuple_c<int>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0, 1>),
                tuple_c<int, 1>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0, 1, 2>),
                tuple_c<int, 1, 2>
            ));
        }

        test::TestIterable<tuple_tag>{eq_tuples};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        auto eq_tuples = make<tuple_tag>(
              make<tuple_tag>()
            , make<tuple_tag>(eq<0>{})
            , make<tuple_tag>(eq<0>{}, eq<1>{})
            , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{})
            , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
            , make<tuple_tag>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
        );
        test::TestSearchable<tuple_tag>{eq_tuples, make<tuple_tag>(eq<3>{}, eq<5>{})};

        auto bool_tuples = make<tuple_tag>(
              make<tuple_tag>(true_c)
            , make<tuple_tag>(false_c)
            , make<tuple_tag>(true_c, true_c)
            , make<tuple_tag>(true_c, false_c)
            , make<tuple_tag>(false_c, true_c)
            , make<tuple_tag>(false_c, false_c)
        );
        test::TestSearchable<tuple_tag>{bool_tuples, make<tuple_tag>(true_c, false_c)};
    }

#elif BOOST_HANA_TEST_PART == 6
    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    {
        // cartesian_product
        {
            static_assert(
                cartesian_product(make_tuple(
                    make_tuple(1),
                    make_tuple('a', 'b')
                )) == make_tuple(
                    make_tuple(1, 'a'),
                    make_tuple(1, 'b')
                )
            , "");

            static_assert(
                cartesian_product(make_tuple(
                    make_tuple(1, 2),
                    make_tuple('a')
                )) == make_tuple(
                    make_tuple(1, 'a'),
                    make_tuple(2, 'a')
                )
            , "");

            static_assert(
                cartesian_product(make_tuple(
                    make_tuple(1, 2),
                    make_tuple('a', 'b')
                )) == make_tuple(
                    make_tuple(1, 'a'),
                    make_tuple(1, 'b'),
                    make_tuple(2, 'a'),
                    make_tuple(2, 'b')
                )
            , "");

            static_assert(
                cartesian_product(make_tuple(
                    make_tuple(1),
                    make_tuple('a'),
                    make_tuple(1.f),
                    make_tuple(1l, 2l)
                )) == make_tuple(
                    make_tuple(1, 'a', 1.f, 1l),
                    make_tuple(1, 'a', 1.f, 2l)
                )
            , "");

            static_assert(
                cartesian_product(make_tuple(
                    make_tuple(1),
                    make_tuple('a'),
                    make_tuple(1.f),
                    make_tuple(1l, 2l),
                    make_tuple(nullptr)
                )) == make_tuple(
                    make_tuple(1, 'a', 1.f, 1l, nullptr),
                    make_tuple(1, 'a', 1.f, 2l, nullptr)
                )
            , "");
        }

        test::TestSequence<tuple_tag>{};
    }

#elif BOOST_HANA_TEST_PART == 7
    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        // transform with tuple_t and a Metafunction
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(tuple_t<>, metafunction<F>),
                tuple_t<>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(tuple_t<x1>, metafunction<F>),
                tuple_t<F<x1>::type>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(tuple_t<x1, x2>, metafunction<F>),
                tuple_t<F<x1>::type, F<x2>::type>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(tuple_t<x1, x2, x3>, metafunction<F>),
                tuple_t<F<x1>::type, F<x2>::type, F<x3>::type>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(tuple_t<x1, x2, x3, x4>, metafunction<F>),
                tuple_t<F<x1>::type, F<x2>::type, F<x3>::type, F<x4>::type>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(tuple_t<x1, x2, x3, x4>, template_<F>),
                tuple_t<F<x1>, F<x2>, F<x3>, F<x4>>
            ));
        }

        // fill with tuple_t and a hana::type
        {
            struct z;
            BOOST_HANA_CONSTANT_CHECK(equal(
                fill(tuple_t<>, type_c<z>),
                tuple_t<>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fill(tuple_t<x1>, type_c<z>),
                tuple_t<z>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fill(tuple_t<x1, x2>, type_c<z>),
                tuple_t<z, z>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                fill(tuple_t<x1, x2, x3>, type_c<z>),
                tuple_t<z, z, z>
            ));
        }

        // laws
        test::TestFunctor<tuple_tag>{eq_tuples, eq_values};
    }

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestApplicative<tuple_tag>{eq_tuples};
    }

#elif BOOST_HANA_TEST_PART == 8
    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestMonad<tuple_tag>{eq_tuples, nested_eqs};
    }

#elif BOOST_HANA_TEST_PART == 9
    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    {
        // prepend
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(tuple_c<long>, long_c<0>),
                tuple_c<long, 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(tuple_c<unsigned int, 1>, uint_c<0>),
                tuple_c<unsigned int, 0, 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(tuple_c<long long, 1, 2>, llong_c<0>),
                tuple_c<long long, 0, 1, 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(tuple_c<unsigned long, 1, 2, 3>, ulong_c<0>),
                tuple_c<unsigned long, 0, 1, 2, 3>
            ));
        }

        // empty
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<tuple_tag>(),
                tuple_c<int>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<tuple_tag>(),
                tuple_c<long>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<tuple_tag>(),
                tuple_c<void>
            ));
        }

        // laws
        test::TestMonadPlus<tuple_tag>{eq_tuples, predicates, eq_values};
    }
#endif
}
