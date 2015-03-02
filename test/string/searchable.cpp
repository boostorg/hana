/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/searchable.hpp>
using namespace boost::hana;


// This test is in its own file to avoid crashing the compiler.

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto strings = make<Tuple>(
        BOOST_HANA_STRING(""),
        BOOST_HANA_STRING("a"),
        BOOST_HANA_STRING("ab"),
        BOOST_HANA_STRING("abcd"),
        BOOST_HANA_STRING("ba"),
        BOOST_HANA_STRING("afcd")
    );

    auto keys = tuple_c<char, 'a', 'f'>;

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                any_of(BOOST_HANA_STRING("abcd"), equal.to(char_<'b'>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(BOOST_HANA_STRING(""), always(true_)))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(BOOST_HANA_STRING("abcd"), equal.to(char_<'z'>)))
            );
        }

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING(""), always(true_)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), equal.to(char_<'a'>)),
                just(char_<'a'>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), equal.to(char_<'c'>)),
                just(char_<'c'>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), equal.to(char_<'d'>)),
                just(char_<'d'>)
            ));
        }

        // elem
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(
                elem(BOOST_HANA_STRING("abcd"), char_<'a'>)
            );
            BOOST_HANA_CONSTANT_CHECK(
                elem(BOOST_HANA_STRING("abcd"), char_<'c'>)
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(elem(BOOST_HANA_STRING("abcd"), char_<'e'>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(elem(BOOST_HANA_STRING("abcd"), invalid{}))
            );
        }

        // lookup
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(equal(
                lookup(BOOST_HANA_STRING("abcd"), char_<'a'>),
                just(char_<'a'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lookup(BOOST_HANA_STRING("abcd"), char_<'c'>),
                just(char_<'c'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                lookup(BOOST_HANA_STRING("abcd"), invalid{}),
                nothing
            ));
        }

        // laws
        test::TestSearchable<String>{strings, keys};
    }
}
