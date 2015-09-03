/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/set.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/searchable.hpp>
#include <test/seq.hpp>
using namespace boost::hana;


using test::ct_eq;
struct undefined { };

int main() {
    auto eqs = make<tuple_tag>(
        make_set(),
        make_set(ct_eq<0>{}),
        make_set(ct_eq<0>{}, ct_eq<1>{}),
        make_set(ct_eq<1>{}, ct_eq<0>{}),
        make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
    );
    (void)eqs;

    auto keys = make_tuple(ct_eq<2>{}, ct_eq<3>{});
    (void)keys;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // make<set_tag>
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<set_tag>(),
            make_set()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<set_tag>(ct_eq<0>{}),
            make_set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<set_tag>(ct_eq<0>{}, ct_eq<1>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<set_tag>(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // Make sure we do not instantiate rogue constructors when doing copies
    //////////////////////////////////////////////////////////////////////////
    {
        auto expr = make<set_tag>(test::trap_construct{});
        auto implicit_copy = expr;          (void)implicit_copy;
        decltype(expr) explicit_copy(expr); (void)explicit_copy;
    }

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make_set(), ct_eq<0>{}),
            make_set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make_set(ct_eq<0>{}), ct_eq<0>{}),
            make_set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make_set(ct_eq<0>{}), ct_eq<1>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make_set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<1>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make_set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<2>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), ct_eq<3>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(), undefined{}),
            make_set()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}), ct_eq<0>{}),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}), ct_eq<99>{}),
            make_set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<0>{}),
            make_set(ct_eq<1>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<1>{}),
            make_set(ct_eq<0>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}), ct_eq<99>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), ct_eq<0>{}),
            make_set(ct_eq<1>{}, ct_eq<2>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), ct_eq<1>{}),
            make_set(ct_eq<0>{}, ct_eq<2>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), ct_eq<2>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}), ct_eq<99>{}),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // intersection
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(),
                make_set()
            ),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}),
                make_set()
            ),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(),
                make_set(ct_eq<0>{})
            ),
            make_set()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<1>{})
            ),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<0>{})
            ),
            make_set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set(ct_eq<2>{}, ct_eq<3>{})
            ),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set(ct_eq<1>{}, ct_eq<2>{})
            ),
            make_set(ct_eq<1>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set(ct_eq<1>{}, ct_eq<0>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}),
                make_set(ct_eq<1>{}, ct_eq<0>{}, ct_eq<4>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            intersection(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}),
                make_set(ct_eq<1>{}, ct_eq<0>{}, ct_eq<3>{}, ct_eq<2>{}, ct_eq<4>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // union_
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(),
                make_set()
            ),
            make_set()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}),
                make_set()
            ),
            make_set(ct_eq<0>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(),
                make_set(ct_eq<0>{})
            ),
            make_set(ct_eq<0>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<0>{})
            ),
            make_set(ct_eq<0>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<1>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set(ct_eq<1>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<1>{}, ct_eq<0>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            union_(
                make_set(ct_eq<0>{}, ct_eq<2>{}, ct_eq<5>{}),
                make_set(ct_eq<1>{}, ct_eq<3>{}, ct_eq<4>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // difference
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(),
                make_set()
            ),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(ct_eq<0>{}),
                make_set()
            ),
            make_set(ct_eq<0>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set()
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set()
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<2>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{}, ct_eq<3>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{}, ct_eq<3>{}, ct_eq<2>{})
            ),
            make_set(ct_eq<0>{})
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // symmetric_difference
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(),
                make_set()
            ),
            make_set()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(ct_eq<0>{}),
                make_set()
            ),
            make_set(ct_eq<0>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set()
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set()
            ),
            make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<2>{})
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{}, ct_eq<3>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<2>{}, ct_eq<3>{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            symmetric_difference(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<1>{}, ct_eq<3>{}, ct_eq<2>{})
            ),
            make_set(ct_eq<0>{}, ct_eq<3>{})
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
                    contains(permutations(list(xs...)), to<L>(make_set(xs...)))
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
                to<set_tag>(foldable()),
                make_set()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{})),
                make_set(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{}, ct_eq<1>{})),
                make_set(ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{}, ct_eq<2>{})),
                make_set(ct_eq<1>{}, ct_eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<1>{})),
                make_set(ct_eq<1>{}, ct_eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<2>{})),
                make_set(ct_eq<1>{}, ct_eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})),
                make_set(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<set_tag>(foldable(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<2>{}, ct_eq<1>{})),
                make_set(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{})
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
                auto keys_set = make_set(keys...);
                auto keys_tuple = make_tuple(keys...);
                auto possible_arrangements = permutations(keys_tuple);

                BOOST_HANA_CONSTANT_CHECK(all_of(possible_arrangements, [&](auto perm) {
                    return equal(to<set_tag>(perm), keys_set);
                }));

                BOOST_HANA_CONSTANT_CHECK(not_(equal(
                    keys_set,
                    make_set(keys..., ct_eq<999>{})
                )));
            };

            BOOST_HANA_CONSTANT_CHECK(make_set(ct_eq<0>{}) == make_set(ct_eq<0>{}));
            BOOST_HANA_CONSTANT_CHECK(make_set(ct_eq<0>{}) != make_set(ct_eq<1>{}));

            check();
            check(ct_eq<0>{});
            check(ct_eq<0>{}, ct_eq<1>{});
            check(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{});
        }

        // laws
        test::TestComparable<set_tag>{eqs};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            make_set(ct_eq<1>{})[ct_eq<1>{}],
            ct_eq<1>{}
        ));

        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make_set(), equal.to(ct_eq<1>{})))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make_set(ct_eq<1>{}), equal.to(ct_eq<1>{}))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make_set(ct_eq<1>{}), equal.to(ct_eq<2>{})))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make_set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<1>{}))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(make_set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<2>{}))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make_set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<3>{})))
            );
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make_set(), equal.to(ct_eq<1>{})),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make_set(ct_eq<1>{}), equal.to(ct_eq<1>{})),
                just(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make_set(ct_eq<1>{}), equal.to(ct_eq<2>{})),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make_set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<1>{})),
                just(ct_eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make_set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<2>{})),
                just(ct_eq<2>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make_set(ct_eq<1>{}, ct_eq<2>{}), equal.to(ct_eq<3>{})),
                nothing
            ));
        }

        // is_subset
        {
            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_set(),
                make_set(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_set(ct_eq<0>{}),
                make_set(ct_eq<0>{}, ct_eq<1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_set(ct_eq<0>{}, ct_eq<1>{}),
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}),
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(not_(is_subset(
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<3>{}),
                make_set(ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{})
            )));
        }

        // laws
        test::TestSearchable<set_tag>{eqs, keys};
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
                contains(possible_results, unpack(make_set(xs...), f))
            );
        };

        check();
        check(ct_eq<1>{});
        check(ct_eq<1>{}, ct_eq<2>{});
        check(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{});
        check(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{});

        // laws
        test::TestFoldable<set_tag>{eqs};
    }
#endif
}
