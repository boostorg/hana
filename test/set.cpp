/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/set.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>
#include <test/seq.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/searchable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto objects<Set> = tuple(
        set(),
        set(x<0>),
        set(x<0>, x<1>),
        set(x<1>, x<0>),
        set(x<0>, x<1>, x<2>)
    );

    template <>
    auto instances<Set> = tuple(
        type<Comparable>,
        type<Searchable>
    );
}}}


int main() {
    test::check_datatype<Set>();

    // make
    {
        using test::x;

        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<Set>(),
            set()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<Set>(x<0>),
            set(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<Set>(x<0>, x<1>),
            set(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<Set>(x<0>, x<1>, x<2>),
            set(x<0>, x<1>, x<2>)
        ));
    }

    // insert
    {
        using test::x;

        BOOST_HANA_CONSTANT_ASSERT(equal(
            insert(set(), x<0>),
            set(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            insert(set(x<0>), x<0>),
            set(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            insert(set(x<0>), x<1>),
            set(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            insert(set(x<0>, x<1>), x<1>),
            set(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            insert(set(x<0>, x<1>), x<2>),
            set(x<0>, x<1>, x<2>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
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

        // Set -> List
        {
            auto check = [=](auto ...xs) {
                BOOST_HANA_CONSTANT_ASSERT(
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
            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable()),
                set()
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable(x<1>)),
                set(x<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable(x<1>, x<1>)),
                set(x<1>)
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable(x<1>, x<2>)),
                set(x<1>, x<2>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable(x<1>, x<2>, x<1>)),
                set(x<1>, x<2>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable(x<1>, x<2>, x<2>)),
                set(x<1>, x<2>)
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                to<Set>(foldable(x<1>, x<2>, x<3>)),
                set(x<1>, x<2>, x<3>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
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
                    all(permutations(tuple(keys...)), [=](auto perm) {
                        return equal(to<Set>(perm), set(keys...));
                    }),
                    not_(equal(set(keys...), set(keys..., x<999>)))
                );
            };

            BOOST_HANA_CONSTANT_ASSERT(check());
            BOOST_HANA_CONSTANT_ASSERT(check(x<0>));
            BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>));
            BOOST_HANA_CONSTANT_ASSERT(check(x<0>, x<1>, x<2>));
        }
    }

    // Searchable
    {
        auto is = [](auto x) {
            return [=](auto y) { return equal(x, y); };
        };
        using test::x;

        // any
        {
            BOOST_HANA_CONSTANT_ASSERT(not_(any(set(), is(x<1>))));

            BOOST_HANA_CONSTANT_ASSERT(any(set(x<1>), is(x<1>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(any(set(x<1>), is(x<2>))));

            BOOST_HANA_CONSTANT_ASSERT(any(set(x<1>, x<2>), is(x<1>)));
            BOOST_HANA_CONSTANT_ASSERT(any(set(x<1>, x<2>), is(x<2>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(any(set(x<1>, x<2>), is(x<3>))));
        }

        // find
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(set(), is(x<1>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(set(x<1>), is(x<1>)),
                just(x<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(set(x<1>), is(x<2>)),
                nothing
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(set(x<1>, x<2>), is(x<1>)),
                just(x<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(set(x<1>, x<2>), is(x<2>)),
                just(x<2>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                find(set(x<1>, x<2>), is(x<3>)),
                nothing
            ));
        }
    }
}
