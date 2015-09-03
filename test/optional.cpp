/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/optional.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/functor.hpp>
#include <laws/monad.hpp>
#include <laws/monad_plus.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>
#include <test/identity.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...>
using void_t = void;

template <typename T, typename = void>
struct has_type : std::false_type { };

template <typename T>
struct has_type<T, void_t<typename T::type>>
    : std::true_type
{ };

struct undefined { };

using test::ct_eq;
using test::ct_ord;

int main() {
    auto ords = make<tuple_tag>(
        nothing, just(ct_ord<0>{}), just(ct_ord<1>{}), just(ct_ord<2>{})
    );
    (void)ords;

    auto eqs = make<tuple_tag>(
        nothing, just(ct_eq<0>{}), just(ct_eq<1>{}), just(ct_eq<2>{})
    );
    (void)eqs;

    auto eq_values = make<tuple_tag>(ct_eq<0>{}, ct_eq<2>{}, ct_eq<3>{});
    (void)eq_values;

    auto predicates = make<tuple_tag>(
        equal.to(ct_eq<0>{}), equal.to(ct_eq<2>{}), equal.to(ct_eq<3>{}),
        always(false_c), always(true_c)
    );
    (void)predicates;

    auto nested_eqs = make<tuple_tag>(
        nothing, just(just(ct_eq<0>{})), just(nothing), just(just(ct_eq<2>{}))
    );
    (void)nested_eqs;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Optional interface
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};
        test::ct_eq<2> x{};
        test::ct_eq<3> y{};

        // Interaction with hana::type (has a nested ::type)
        {
            struct T;
            static_assert(std::is_same<decltype(just(type_c<T>))::type, T>{}, "");
            static_assert(!has_type<decltype(nothing)>{}, "");
        }

        // maybe
        {
            BOOST_HANA_CONSTANT_CHECK(equal(maybe(x, undefined{}, nothing), x));
            BOOST_HANA_CONSTANT_CHECK(equal(maybe(undefined{}, f, just(x)), f(x)));
        }

        // is_nothing
        {
            BOOST_HANA_CONSTANT_CHECK(is_nothing(nothing));
            BOOST_HANA_CONSTANT_CHECK(not_(is_nothing(just(undefined{}))));
        }

        // is_just
        {
            BOOST_HANA_CONSTANT_CHECK(is_just(just(undefined{})));
            BOOST_HANA_CONSTANT_CHECK(not_(is_just(nothing)));
        }

        // from_just
        {
            BOOST_HANA_CONSTANT_CHECK(equal(from_just(just(x)), x));

            {
                auto lvalue = just(test::ct_eq<3>{});
                test::ct_eq<3>& ref = *lvalue;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ref,
                    test::ct_eq<3>{}
                ));

                auto const const_lvalue = just(test::ct_eq<3>{});
                test::ct_eq<3> const& const_ref = *const_lvalue;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    const_ref,
                    test::ct_eq<3>{}
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    *just(test::ct_eq<3>{}),
                    test::ct_eq<3>{}
                ));
            }

            {
                struct object {
                    test::ct_eq<3> member;
                };

                auto lvalue = just(object{});
                test::ct_eq<3>& ref = lvalue->member;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    ref,
                    test::ct_eq<3>{}
                ));

                auto const const_lvalue = just(object{});
                test::ct_eq<3> const& const_ref = const_lvalue->member;
                BOOST_HANA_CONSTANT_CHECK(equal(
                    const_ref,
                    test::ct_eq<3>{}
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    just(object{})->member,
                    test::ct_eq<3>{}
                ));
            }
        }

        // from_maybe
        {
            BOOST_HANA_CONSTANT_CHECK(equal(from_maybe(x, nothing), x));
            BOOST_HANA_CONSTANT_CHECK(equal(from_maybe(undefined{}, just(y)), y));
        }

        // only_when
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                only_when(always(true_c), f, x),
                just(f(x))
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                only_when(always(false_c), f, x),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                only_when(always(false_c), undefined{}, x),
                nothing
            ));
        }

        // sfinae
        {
            test::_injection<0> f{};

            BOOST_HANA_CONSTANT_CHECK(equal(
                sfinae(f)(),
                just(f())
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sfinae(f)(ct_eq<0>{}),
                just(f(ct_eq<0>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sfinae(f)(ct_eq<0>{}, ct_eq<1>{}),
                just(f(ct_eq<0>{}, ct_eq<1>{}))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sfinae(undefined{})(),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sfinae(undefined{})(ct_eq<0>{}),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sfinae(undefined{})(ct_eq<0>{}, ct_eq<1>{}),
                nothing
            ));

            BOOST_HANA_CONSTEXPR_LAMBDA auto incr = sfinae([](auto x) -> decltype(x + 1) {
                return x + 1;
            });

            BOOST_HANA_CONSTEXPR_CHECK(equal(
                incr(1), just(2)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                incr(undefined{}), nothing
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                chain(just(1), incr), just(2)
            ));

            // using `sfinae` with a non-pod argument used to fail
            sfinae(undefined{})(Tracked{1});
            sfinae([t = Tracked{1}](auto) { return 1; })(Tracked{1});
        }

        // Make sure we do not instantiate rogue contructors when trying
        // to copy a just.
        {
            auto expr = just(test::trap_construct{});
            auto implicit_copy = expr;          (void)implicit_copy;
            decltype(expr) explicit_copy(expr); (void)explicit_copy;
        }

        // make<optional_tag>
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<optional_tag>(),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<optional_tag>(ct_eq<0>{}),
                just(ct_eq<0>{})
            ));
        }

        // make_optional
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make_optional(),
                make<optional_tag>()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make_optional(ct_eq<0>{}),
                make<optional_tag>(ct_eq<0>{})
            ));
        }
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            test::ct_eq<3> x{};
            test::ct_eq<4> y{};

            BOOST_HANA_CONSTANT_CHECK(nothing == nothing);
            BOOST_HANA_CONSTANT_CHECK(just(x) == just(x));
            BOOST_HANA_CONSTANT_CHECK(just(x) != just(y));
            BOOST_HANA_CONSTANT_CHECK(just(x) != nothing);
            BOOST_HANA_CONSTANT_CHECK(nothing != just(x));

            BOOST_HANA_CONSTANT_CHECK(equal(nothing, nothing));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(nothing, just(x))));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(just(x), nothing)));
            BOOST_HANA_CONSTANT_CHECK(equal(just(x), just(x)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(just(x), just(y))));
        }

        // laws
        test::TestComparable<optional_tag>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        // less
        {
            BOOST_HANA_CONSTANT_CHECK(nothing < just(undefined{}));
            BOOST_HANA_CONSTANT_CHECK(just(ct_ord<3>{}) < just(ct_ord<4>{}));
            BOOST_HANA_CONSTANT_CHECK(just(ct_ord<3>{}) <= just(ct_ord<4>{}));
            BOOST_HANA_CONSTANT_CHECK(just(ct_ord<4>{}) > just(ct_ord<3>{}));
            BOOST_HANA_CONSTANT_CHECK(just(ct_ord<4>{}) >= just(ct_ord<3>{}));

            BOOST_HANA_CONSTANT_CHECK(less(
                nothing,
                just(undefined{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                just(undefined{}),
                nothing
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                nothing,
                nothing
            )));

            BOOST_HANA_CONSTANT_CHECK(less(
                just(ct_ord<3>{}),
                just(ct_ord<4>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                just(ct_ord<3>{}),
                just(ct_ord<3>{})
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(less(
                just(ct_ord<4>{}),
                just(ct_ord<3>{})
            )));
        }

        // laws
        test::TestOrderable<optional_tag>{ords};
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
                transform(nothing, f),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(just(ct_eq<3>{}), f),
                just(f(ct_eq<3>{}))
            ));
        }

        // laws
        test::TestFunctor<optional_tag>{eqs, eq_values};
    }

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(nothing, nothing),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(just(f), nothing),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(nothing, just(ct_eq<3>{})),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(just(f), just(ct_eq<3>{})),
                just(f(ct_eq<3>{}))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<optional_tag>(ct_eq<3>{}),
                just(ct_eq<3>{})
            ));
        }

        // laws
        test::TestApplicative<optional_tag>{eqs};
    }

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        BOOST_HANA_CONSTANT_CHECK(equal(
            just(ct_eq<3>{}) | f,
            f(ct_eq<3>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            nothing | f,
            nothing
        ));

        // flatten
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(nothing),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(just(nothing)),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                flatten(just(just(ct_eq<4>{}))),
                just(ct_eq<4>{})
            ));
        }

        // laws
        test::TestMonad<optional_tag>{eqs, nested_eqs};
    }

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    {
        // empty
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<optional_tag>(),
                nothing
            ));
        }

        // concat
        {
            auto rv_nothing = [] { return nothing; }; // rvalue nothing

            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(rv_nothing(), nothing),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(nothing, rv_nothing()),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(rv_nothing(), rv_nothing()),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(rv_nothing(), just(ct_eq<0>{})),
                just(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(just(ct_eq<0>{}), rv_nothing()),
                just(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(nothing, nothing), nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(nothing, just(ct_eq<0>{})),
                just(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(just(ct_eq<0>{}), nothing),
                just(ct_eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                concat(just(ct_eq<0>{}), just(ct_eq<1>{})),
                just(ct_eq<0>{})
            ));
        }

        // laws
        test::TestMonadPlus<optional_tag>{eqs, predicates, eq_values};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        test::ct_eq<2> x{};
        test::ct_eq<3> y{};

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(just(x), equal.to(x)),
                just(x)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(just(x), equal.to(y)),
                nothing
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(nothing, equal.to(x)),
                nothing
            ));

            // Previously, there was a bug that would make this fail.
            auto non_const_nothing = nothing;
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(non_const_nothing, equal.to(x)),
                nothing
            ));
        }

        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(any_of(just(x), equal.to(x)));
            BOOST_HANA_CONSTANT_CHECK(not_(any_of(just(x), equal.to(y))));
            BOOST_HANA_CONSTANT_CHECK(not_(any_of(nothing, equal.to(x))));
        }

        // laws
        test::TestSearchable<optional_tag>{eqs, eq_values};

        test::TestSearchable<optional_tag>{
            make<tuple_tag>(just(true_c), just(false_c), nothing),
            make<tuple_tag>(true_c, false_c)
        };
    }

#elif BOOST_HANA_TEST_PART == 6
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::ct_eq<2> x{};
        test::ct_eq<3> s{};
        test::_injection<0> f{};

        // fold_left
        {
            BOOST_HANA_CONSTANT_CHECK(equal(fold_left(just(x), s, f), f(s, x)));
            BOOST_HANA_CONSTANT_CHECK(equal(fold_left(nothing, s, f), s));
        }

        // fold_right
        {
            BOOST_HANA_CONSTANT_CHECK(equal(fold_right(just(x), s, f), f(x, s)));
            BOOST_HANA_CONSTANT_CHECK(equal(fold_right(nothing, s, f), s));
        }

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(unpack(nothing, f), f()));
            BOOST_HANA_CONSTANT_CHECK(equal(unpack(just(x), f), f(x)));

            // Previously, there was a bug that would make this fail.
            auto non_const_nothing = nothing;
            BOOST_HANA_CONSTANT_CHECK(equal(unpack(non_const_nothing, f), f()));
        }

        // laws
        test::TestFoldable<optional_tag>{eqs};
    }
#endif
}
