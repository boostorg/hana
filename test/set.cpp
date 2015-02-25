/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/searchable.hpp>
#include <test/injection.hpp>
#include <test/seq.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto objects<Set> = make<Tuple>(
        set(),
        set(x<0>),
        set(x<0>, x<1>),
        set(x<1>, x<0>),
        set(x<0>, x<1>, x<2>)
    );

    template <>
    auto instances<Set> = make<Tuple>(
        type<Comparable>,
        type<Foldable>,
        type<Searchable>
    );
}}}


int main() {
    test::check_datatype<Set>();

    // make
    {
        using test::x;

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(),
            set()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(x<0>),
            set(x<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(x<0>, x<1>),
            set(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<Set>(x<0>, x<1>, x<2>),
            set(x<0>, x<1>, x<2>)
        ));
    }

    // insert
    {
        using test::x;

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(), x<0>),
            set(x<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(x<0>), x<0>),
            set(x<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(x<0>), x<1>),
            set(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(x<0>, x<1>), x<1>),
            set(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(x<0>, x<1>), x<2>),
            set(x<0>, x<1>, x<2>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            insert(set(x<0>, x<1>, x<2>), x<3>),
            set(x<0>, x<1>, x<2>, x<3>)
        ));
    }

    // Conversions
    {
        auto list = test::seq;
        auto foldable = test::seq;
        using L = test::Seq;
        using test::x;

        // Set -> Sequence (now provided by Sequence, but we keep the test)
        {
            auto check = [=](auto ...xs) {
                BOOST_HANA_CONSTANT_CHECK(
                    elem(permutations(list(xs...)), to<L>(set(xs...)))
                );
            };
            check();
            check(x<1>);
            check(x<1>, x<2>);
            check(x<1>, x<2>, x<3>);
            check(x<1>, x<2>, x<3>, x<4>);
        }

        // Foldable -> Set
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable()),
                set()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>)),
                set(x<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>, x<1>)),
                set(x<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>, x<2>)),
                set(x<1>, x<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>, x<2>, x<1>)),
                set(x<1>, x<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>, x<2>, x<2>)),
                set(x<1>, x<2>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>, x<2>, x<3>)),
                set(x<1>, x<2>, x<3>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                to<Set>(foldable(x<1>, x<2>, x<3>, x<2>, x<1>)),
                set(x<1>, x<2>, x<3>)
            ));
        }
    }

    // Comparable
    {
        using test::x;

        // equal
        {
            BOOST_HANA_CONSTEXPR_LAMBDA auto check = [](auto ...keys) {
                return and_(
                    all(permutations(make<Tuple>(keys...)), [=](auto perm) {
                        return equal(to<Set>(perm), set(keys...));
                    }),
                    not_(equal(set(keys...), set(keys..., x<999>)))
                );
            };

            BOOST_HANA_CONSTANT_CHECK(check());
            BOOST_HANA_CONSTANT_CHECK(check(x<0>));
            BOOST_HANA_CONSTANT_CHECK(check(x<0>, x<1>));
            BOOST_HANA_CONSTANT_CHECK(check(x<0>, x<1>, x<2>));
        }
    }

    // Searchable
    {
        using test::x;

        // any
        {
            BOOST_HANA_CONSTANT_CHECK(not_(any(set(), equal.to(x<1>))));

            BOOST_HANA_CONSTANT_CHECK(any(set(x<1>), equal.to(x<1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(any(set(x<1>), equal.to(x<2>))));

            BOOST_HANA_CONSTANT_CHECK(any(set(x<1>, x<2>), equal.to(x<1>)));
            BOOST_HANA_CONSTANT_CHECK(any(set(x<1>, x<2>), equal.to(x<2>)));
            BOOST_HANA_CONSTANT_CHECK(not_(any(set(x<1>, x<2>), equal.to(x<3>))));
        }

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(set(), equal.to(x<1>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(set(x<1>), equal.to(x<1>)),
                just(x<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(set(x<1>), equal.to(x<2>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(set(x<1>, x<2>), equal.to(x<1>)),
                just(x<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(set(x<1>, x<2>), equal.to(x<2>)),
                just(x<2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(set(x<1>, x<2>), equal.to(x<3>)),
                nothing
            ));
        }
    }

    // Foldable
    {
        auto list = test::seq;
        using test::x;
        auto f = test::injection([]{});

        auto check = [=](auto ...xs) {
            auto possible_results = transform(permutations(list(xs...)), [=](auto perm) {
                return unpack(perm, f);
            });

            BOOST_HANA_CONSTANT_CHECK(
                elem(possible_results, unpack(set(xs...), f))
            );
        };

        check();
        check(x<1>);
        check(x<1>, x<2>);
        check(x<1>, x<2>, x<3>);
        check(x<1>, x<2>, x<3>, x<4>);
    }
}
