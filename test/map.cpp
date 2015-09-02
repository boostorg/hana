/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/map.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/searchable.hpp>
#include <test/minimal_product.hpp>
#include <test/seq.hpp>

#include <utility>
using namespace boost::hana;


template <int i>
auto key() { return test::ct_eq<i>{}; }

template <int i>
auto val() { return test::ct_eq<-i>{}; }

template <int i, int j>
auto p() { return test::minimal_product(key<i>(), val<j>()); }

struct undefined { };

int main() {
    auto list = test::seq; (void)list;
    auto foldable = test::seq; (void)foldable;
    using L = test::Seq;

    auto eq_maps = make<tuple_tag>(
        make_map(),
        make_map(p<1, 1>()),
        make_map(p<1, 2>()),
        make_map(p<1, 1>(), p<2, 2>()),
        make_map(p<1, 1>(), p<2, 2>(), p<3, 3>())
    );
    (void)eq_maps;

    auto eq_keys = make<tuple_tag>(key<1>(), key<4>());
    (void)eq_keys;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Make sure we do not instantiate rogue constructors when doing copies
    //////////////////////////////////////////////////////////////////////////
    {
        auto expr = make<map_tag>(
            test::minimal_product(test::trap_construct{}, test::trap_construct{})
        );
        auto implicit_copy = expr;          (void)implicit_copy;
        decltype(expr) explicit_copy(expr); (void)explicit_copy;
    }

    //////////////////////////////////////////////////////////////////////////
    // keys
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(make<map_tag>()),
            list()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(make<map_tag>(p<1, 1>())),
            list(key<1>())
        ));
        BOOST_HANA_CONSTANT_CHECK(
            keys(make<map_tag>(p<1, 1>(), p<2, 2>()))
                ^in^ permutations(list(key<1>(), key<2>()))
        );
        BOOST_HANA_CONSTANT_CHECK(
            keys(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()))
                ^in^ permutations(list(key<1>(), key<2>(), key<3>()))
        );
    }

    //////////////////////////////////////////////////////////////////////////
    // values
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            values(make<map_tag>()),
            list()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            values(make<map_tag>(p<1, 1>())),
            list(val<1>())
        ));
        BOOST_HANA_CONSTANT_CHECK(
            values(make<map_tag>(p<1, 1>(), p<2, 2>()))
                ^in^ permutations(list(val<1>(), val<2>()))
        );
        BOOST_HANA_CONSTANT_CHECK(
            values(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()))
                ^in^ permutations(list(val<1>(), val<2>(), val<3>()))
        );
    }

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(), p<1, 1>()),
            make<map_tag>(p<1, 1>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>()), p<1, 99>()),
            make<map_tag>(p<1, 1>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>()), p<2, 2>()),
            make<map_tag>(p<1, 1>(), p<2, 2>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>()), p<1, 99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>()), p<2, 99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>()), p<3, 3>()),
            make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<1, 99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<2, 99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<3, 99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<4, 4>()),
            make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>(), p<4, 4>())
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(), undefined{}),
            make<map_tag>()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>()), key<1>()),
            make<map_tag>()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>()), key<99>()),
            make<map_tag>(p<1, 1>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>()), key<99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>()), key<1>()),
            make<map_tag>(p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>()), key<2>()),
            make<map_tag>(p<1, 1>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<99>()),
            make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<1>()),
            make<map_tag>(p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<2>()),
            make<map_tag>(p<1, 1>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<3>()),
            make<map_tag>(p<1, 1>(), p<2, 2>())
        ));
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    {
        // Foldable -> Map
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable()),
                make<map_tag>()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>())),
                make<map_tag>(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>(), p<2, 2>())),
                make<map_tag>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>(), p<2, 2>(), p<3, 3>())),
                make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
            ));

            // with duplicates
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>(), p<1, 99>())),
                make<map_tag>(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>(), p<2, 2>(), p<1, 99>())),
                make<map_tag>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>(), p<2, 2>(), p<1, 99>(), p<2, 99>())),
                make<map_tag>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<map_tag>(foldable(p<1, 1>(), p<2, 2>(), p<1, 99>(), p<2, 99>(), p<3, 3>())),
                make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>())
            ));
        }

        // Map -> Sequence
        {
            auto check = [=](auto ...xs) {
                BOOST_HANA_CONSTANT_CHECK(
                    to<L>(make<map_tag>(xs...)) ^in^ permutations(list(xs...))
                );
            };
            check();
            check(p<1, 1>());
            check(p<1, 1>(), p<2, 2>());
            check(p<1, 1>(), p<2, 2>(), p<3, 3>());
            check(p<1, 1>(), p<2, 2>(), p<3, 3>(), p<4, 4>());
        }
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(
                make<map_tag>(p<2, 2>(), p<1, 1>()) == make<map_tag>(p<1, 1>(), p<2, 2>())
            );

            BOOST_HANA_CONSTANT_CHECK(
                make<map_tag>(p<1, 1>()) != make<map_tag>(p<1, 1>(), p<2, 2>())
            );
        }

        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<map_tag>(),
                make<map_tag>()
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>()),
                make<map_tag>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(),
                make<map_tag>(p<1, 1>())
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<map_tag>(p<1, 1>()),
                make<map_tag>(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>()),
                make<map_tag>(p<1, 2>())
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>()),
                make<map_tag>(p<2, 1>())
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>()),
                make<map_tag>(p<1, 1>(), p<2, 2>())
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<2, 2>(), p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<9, 1>(), p<2, 2>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<1, 9>(), p<2, 2>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<1, 1>(), p<9, 2>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<1, 1>(), p<2, 9>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<map_tag>(p<1, 1>(), p<2, 2>()),
                make<map_tag>(p<1, 1>(), p<2, 2>(), p<3, 3>()))
            ));
        }

        // laws
        test::TestComparable<map_tag>{eq_maps};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        {
            auto m = make<map_tag>(p<2, 2>(), p<1, 1>());
            auto const const_m = make<map_tag>(p<2, 2>(), p<1, 1>());
            BOOST_HANA_CONSTANT_CHECK(equal(m[key<1>()], val<1>()));
            BOOST_HANA_CONSTANT_CHECK(equal(const_m[key<1>()], val<1>()));
            BOOST_HANA_CONSTANT_CHECK(equal(std::move(m)[key<1>()], val<1>()));
        }

        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<map_tag>(), equal.to(key<1>())))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<map_tag>(p<1, 1>()), equal.to(key<1>()))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<map_tag>(p<1, 1>()), equal.to(key<2>())))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<map_tag>(p<1, 1>(), p<2, 2>()), equal.to(key<1>()))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<map_tag>(p<1, 1>(), p<2, 2>()), equal.to(key<2>()))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<map_tag>(p<1, 1>(), p<2, 2>()), equal.to(key<3>())))
            );
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<map_tag>(), equal.to(key<1>())),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<map_tag>(p<1, 1>()), equal.to(key<1>())),
                just(val<1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<map_tag>(p<1, 1>()), equal.to(key<2>())),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<map_tag>(p<1, 1>(), p<2, 2>()), equal.to(key<1>())),
                just(val<1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<map_tag>(p<1, 1>(), p<2, 2>()), equal.to(key<2>())),
                just(val<2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<map_tag>(p<1, 1>(), p<2, 2>()), equal.to(key<3>())),
                nothing
            ));
        }

        // is_subset
        {
            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_map(),
                make_map()
            ));
            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_map(),
                make_map(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_map(),
                make_map(p<1, 1>(), p<2, 2>())
            ));

            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_map(p<1, 1>()),
                make_map(p<1, 1>(), p<2, 2>())
            ));

            BOOST_HANA_CONSTANT_CHECK(is_subset(
                make_map(p<2, 2>(), p<1, 1>()),
                make_map(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(is_subset(
                make_map(p<3, 3>(), p<1, 1>()),
                make_map(p<1, 1>(), p<2, 2>())
            )));
        }

        // laws
        test::TestSearchable<map_tag>{eq_maps, eq_keys};
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
#elif BOOST_HANA_TEST_PART == 5
    // fold_left
    {
        // Use pointers to workaround a Clang ICE
        test::_injection<0> f{}; auto* fp = &f;
        test::ct_eq<999> state{}; auto* statep = &state;

        auto check = [=](auto ...pairs) {
            BOOST_HANA_CONSTANT_CHECK(
                fold_left(make<map_tag>(pairs...), state, f)
                    ^in^
                transform(permutations(list(pairs...)), [=](auto xs) {
                    return fold_left(xs, *statep, *fp);
                })
            );
        };

        BOOST_HANA_CONSTANT_CHECK(equal(
            fold_left(make<map_tag>(), state, undefined{}),
            state
        ));

        check(p<1, 1>());
        check(p<1, 1>(), p<2, 2>());
        check(p<1, 1>(), p<2, 2>(), p<3, 3>());
        check(p<1, 1>(), p<2, 2>(), p<3, 3>(), p<4, 4>());
    }

#elif BOOST_HANA_TEST_PART == 6
    // fold_right
    {
        test::_injection<0> f{}; auto* fp = &f;
        test::ct_eq<999> state{}; auto* statep = &state;

        auto check = [=](auto ...pairs) {
            BOOST_HANA_CONSTANT_CHECK(
                fold_right(make<map_tag>(pairs...), state, f)
                    ^in^
                transform(permutations(list(pairs...)), [=](auto xs) {
                    return fold_right(xs, *statep, *fp);
                })
            );
        };

        BOOST_HANA_CONSTANT_CHECK(equal(
            fold_right(make<map_tag>(), state, undefined{}),
            state
        ));

        check(p<1, 1>());
        check(p<1, 1>(), p<2, 2>());
        check(p<1, 1>(), p<2, 2>(), p<3, 3>());
        check(p<1, 1>(), p<2, 2>(), p<3, 3>(), p<4, 4>());
    }

#elif BOOST_HANA_TEST_PART == 7
    // unpack
    {
        test::_injection<0> f{};

        auto check = [=](auto ...pairs) {
            BOOST_HANA_CONSTANT_CHECK(
                unpack(make<map_tag>(pairs...), f)
                    ^in^
                transform(permutations(list(pairs...)), [=](auto xs) {
                    return unpack(xs, f);
                })
            );
        };

        check();
        check(p<1, 1>());
        check(p<1, 1>(), p<2, 2>());
        check(p<1, 1>(), p<2, 2>(), p<3, 3>());
        check(p<1, 1>(), p<2, 2>(), p<3, 3>(), p<4, 4>());
    }

    // laws
    test::TestFoldable<map_tag>{eq_maps};
#endif
}
