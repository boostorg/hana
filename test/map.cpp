/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

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

    auto eq_maps = make<Tuple>(
        make<Map>(),
        make<Map>(p<1, 1>()),
        make<Map>(p<1, 2>()),
        make<Map>(p<1, 1>(), p<2, 2>()),
        make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
    );
    (void)eq_maps;

    auto eq_keys = make<Tuple>(key<1>(), key<4>());
    (void)eq_keys;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Make sure we do not instantiate rogue constructors when doing copies
    //////////////////////////////////////////////////////////////////////////
    {
        auto expr = make<Map>(
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
            keys(make<Map>()),
            list()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            keys(make<Map>(p<1, 1>())),
            list(key<1>())
        ));
        BOOST_HANA_CONSTANT_CHECK(
            keys(make<Map>(p<1, 1>(), p<2, 2>()))
                ^in^ permutations(list(key<1>(), key<2>()))
        );
        BOOST_HANA_CONSTANT_CHECK(
            keys(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()))
                ^in^ permutations(list(key<1>(), key<2>(), key<3>()))
        );
    }

    //////////////////////////////////////////////////////////////////////////
    // values
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            values(make<Map>()),
            list()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            values(make<Map>(p<1, 1>())),
            list(val<1>())
        ));
        BOOST_HANA_CONSTANT_CHECK(
            values(make<Map>(p<1, 1>(), p<2, 2>()))
                ^in^ permutations(list(val<1>(), val<2>()))
        );
        BOOST_HANA_CONSTANT_CHECK(
            values(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()))
                ^in^ permutations(list(val<1>(), val<2>(), val<3>()))
        );
    }

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(), p<1, 1>()),
            make<Map>(p<1, 1>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>()), p<1, 99>()),
            make<Map>(p<1, 1>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>()), p<2, 2>()),
            make<Map>(p<1, 1>(), p<2, 2>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>()), p<1, 99>()),
            make<Map>(p<1, 1>(), p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>()), p<2, 99>()),
            make<Map>(p<1, 1>(), p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>()), p<3, 3>()),
            make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<1, 99>()),
            make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<2, 99>()),
            make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<3, 99>()),
            make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), p<4, 4>()),
            make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>(), p<4, 4>())
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(), undefined{}),
            make<Map>()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>()), key<1>()),
            make<Map>()
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>()), key<99>()),
            make<Map>(p<1, 1>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>()), key<99>()),
            make<Map>(p<1, 1>(), p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>()), key<1>()),
            make<Map>(p<2, 2>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>()), key<2>()),
            make<Map>(p<1, 1>())
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<99>()),
            make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<1>()),
            make<Map>(p<2, 2>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<2>()),
            make<Map>(p<1, 1>(), p<3, 3>())
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            erase_key(make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()), key<3>()),
            make<Map>(p<1, 1>(), p<2, 2>())
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
                to<Map>(foldable()),
                make<Map>()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>())),
                make<Map>(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>(), p<2, 2>())),
                make<Map>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>(), p<2, 2>(), p<3, 3>())),
                make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
            ));

            // with duplicates
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>(), p<1, 99>())),
                make<Map>(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>(), p<2, 2>(), p<1, 99>())),
                make<Map>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>(), p<2, 2>(), p<1, 99>(), p<2, 99>())),
                make<Map>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Map>(foldable(p<1, 1>(), p<2, 2>(), p<1, 99>(), p<2, 99>(), p<3, 3>())),
                make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>())
            ));
        }

        // Map -> Sequence
        {
            auto check = [=](auto ...xs) {
                BOOST_HANA_CONSTANT_CHECK(
                    to<L>(make<Map>(xs...)) ^in^ permutations(list(xs...))
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
                make<Map>(p<2, 2>(), p<1, 1>()) == make<Map>(p<1, 1>(), p<2, 2>())
            );

            BOOST_HANA_CONSTANT_CHECK(
                make<Map>(p<1, 1>()) != make<Map>(p<1, 1>(), p<2, 2>())
            );
        }

        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(),
                make<Map>()
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>()),
                make<Map>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(),
                make<Map>(p<1, 1>())
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>()),
                make<Map>(p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>()),
                make<Map>(p<1, 2>())
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>()),
                make<Map>(p<2, 1>())
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>()),
                make<Map>(p<1, 1>(), p<2, 2>())
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<1, 1>(), p<2, 2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<2, 2>(), p<1, 1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<9, 1>(), p<2, 2>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<1, 9>(), p<2, 2>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<1, 1>(), p<9, 2>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<1, 1>(), p<2, 9>()))
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                make<Map>(p<1, 1>(), p<2, 2>()),
                make<Map>(p<1, 1>(), p<2, 2>(), p<3, 3>()))
            ));
        }

        // laws
        test::TestComparable<Map>{eq_maps};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // operators
        {
            auto m = make<Map>(p<2, 2>(), p<1, 1>());
            auto const const_m = make<Map>(p<2, 2>(), p<1, 1>());
            BOOST_HANA_CONSTANT_CHECK(equal(m[key<1>()], val<1>()));
            BOOST_HANA_CONSTANT_CHECK(equal(const_m[key<1>()], val<1>()));
            BOOST_HANA_CONSTANT_CHECK(equal(std::move(m)[key<1>()], val<1>()));
        }

        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(), equal.to(key<1>())))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>()), equal.to(key<1>()))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(p<1, 1>()), equal.to(key<2>())))
            );

            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>(), p<2, 2>()), equal.to(key<1>()))
            );
            BOOST_HANA_CONSTANT_CHECK(
                any_of(make<Map>(p<1, 1>(), p<2, 2>()), equal.to(key<2>()))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(make<Map>(p<1, 1>(), p<2, 2>()), equal.to(key<3>())))
            );
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(), equal.to(key<1>())),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>()), equal.to(key<1>())),
                just(val<1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>()), equal.to(key<2>())),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>(), p<2, 2>()), equal.to(key<1>())),
                just(val<1>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>(), p<2, 2>()), equal.to(key<2>())),
                just(val<2>())
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(make<Map>(p<1, 1>(), p<2, 2>()), equal.to(key<3>())),
                nothing
            ));
        }

        // laws
        test::TestSearchable<Map>{eq_maps, eq_keys};
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
#elif BOOST_HANA_TEST_PART == 5
    // fold_left
    {
        test::_injection<0> f{};
        test::ct_eq<999> state{};

        auto check = [=](auto ...pairs) {
            BOOST_HANA_CONSTANT_CHECK(
                fold_left(make<Map>(pairs...), state, f)
                    ^in^
                transform(permutations(list(pairs...)), [=](auto xs) {
                    return fold_left(xs, state, f);
                })
            );
        };

        BOOST_HANA_CONSTANT_CHECK(equal(
            fold_left(make<Map>(), state, undefined{}),
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
        test::_injection<0> f{};
        test::ct_eq<999> state{};

        auto check = [=](auto ...pairs) {
            BOOST_HANA_CONSTANT_CHECK(
                fold_right(make<Map>(pairs...), state, f)
                    ^in^
                transform(permutations(list(pairs...)), [=](auto xs) {
                    return fold_right(xs, state, f);
                })
            );
        };

        BOOST_HANA_CONSTANT_CHECK(equal(
            fold_right(make<Map>(), state, undefined{}),
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
                unpack(make<Map>(pairs...), f)
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
    test::TestFoldable<Map>{eq_maps};
#endif
}
