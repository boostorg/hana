/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/searchable.hpp>
#include <test/seq.hpp>
using namespace boost::hana;


using test::ct_eq;

int main() {
    auto eqs = make<Tuple>(
        make<Set>(),
        make<Set>(ct_eq<0>{}),
        make<Set>(ct_eq<0>{}, ct_eq<1>{}),
        make<Set>(ct_eq<1>{}, ct_eq<0>{}),
        make<Set>(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
    );
    (void)eqs;

    auto keys = make<Tuple>(ct_eq<2>{}, ct_eq<3>{});
    (void)keys;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // make<Set>
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(),
            set()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(ct_eq<0>{}),
            set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(ct_eq<0>{}, ct_eq<1>{}),
            set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
            set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Make sure we do not instantiate rogue constructors when doing copies
    //////////////////////////////////////////////////////////////////////////
    {
        auto expr = make<Set>(test::trap_construct{});
        auto implicit_copy = expr;          (void)implicit_copy;
        decltype(expr) explicit_copy(expr); (void)explicit_copy;
    }

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(), ct_eq<0>{}),
            set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(ct_eq<0>{}), ct_eq<0>{}),
            set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(ct_eq<0>{}), ct_eq<1>{}),
            set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<1>{}),
            set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<2>{}),
            set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), ct_eq<3>{}),
            set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    {
        auto list = test::seq;
        auto foldable = test::seq;
        using L = test::Seq;

        // Set -> Sequence
        {
            auto check = [=](auto ...xs) {
                BOOST_HANA_CONSTANT_CHECK(
                    elem(permutations(list(xs...)), to<L>(set(xs...)))
                );
            };
            check();
            check(ct_eq<1>{});
            check(ct_eq<1>{}, ct_eq<2>{});
            check(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{});
            check(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{});
        }

        // Foldable -> Set
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable()),
                set()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{})),
                set(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{}, ct_eq<1>{})),
                set(ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{}, ct_eq<2>{})),
                set(ct_eq<1>{}, ct_eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<1>{})),
                set(ct_eq<1>{}, ct_eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<2>{})),
                set(ct_eq<1>{}, ct_eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})),
                set(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<2>{}, ct_eq<1>{})),
                set(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
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
            auto check = [](auto ...keys) {
                return and_(
                    all_of(permutations(make<Tuple>(keys...)), [=](auto perm) {
                        return equal(to<Set>(perm), set(keys...));
                    }),
                    not_(equal(set(keys...), set(keys..., ct_eq<999>{})))
                );
            };

            BOOST_HANA_CONSTANT_CHECK(check());
            BOOST_HANA_CONSTANT_CHECK(check(ct_eq<0>{}));
            BOOST_HANA_CONSTANT_CHECK(check(ct_eq<0>{}, ct_eq<1>{}));
            BOOST_HANA_CONSTANT_CHECK(check(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}));
        }

        // laws
        test::TestComparable<Set>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(set(), equal.to(ct_eq<1>{})))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(set(ct_eq<1>{}), equal.to(ct_eq<1>{}))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(set(ct_eq<1>{}), equal.to(ct_eq<2>{})))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<1>{}))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<2>{}))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<3>{})))
            );
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(set(), equal.to(ct_eq<1>{})),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(set(ct_eq<1>{}), equal.to(ct_eq<1>{})),
                just(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(set(ct_eq<1>{}), equal.to(ct_eq<2>{})),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<1>{})),
                just(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<2>{})),
                just(ct_eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<3>{})),
                nothing
            ));
        }

        // laws
        static_assert(has_operator<Set, decltype(find)>, "");
        test::TestSearchable<Set>{eqs, keys};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        test::_injection<0> f{};

        auto check = [=](auto ...xs) {
            auto arg_perms = permutations(make_tuple(xs...));
            auto possible_results = transform(arg_perms, [=](auto args) {
                return unpack(args, f);
            });

            BOOST_HANA_CONSTANT_CHECK(
                elem(possible_results, unpack(set(xs...), f))
            );
        };

        check();
        check(ct_eq<1>{});
        check(ct_eq<1>{}, ct_eq<2>{});
        check(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{});
        check(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{});

        // laws
        test::TestFoldable<Set>{eqs};
    }
#endif
}
