/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/iterable.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>

#include <cstring>
#include <type_traits>
using namespace boost::hana;


int main() {
    auto strings = make<tuple_tag>(
        BOOST_HANA_STRING(""),
        BOOST_HANA_STRING("a"),
        BOOST_HANA_STRING("ab"),
        BOOST_HANA_STRING("abc"),
        BOOST_HANA_STRING("abcd"),
        BOOST_HANA_STRING("abcde"),
        BOOST_HANA_STRING("ba"),
        BOOST_HANA_STRING("afcd")
    );
    (void)strings;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // string and BOOST_HANA_STRING
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto const s1 = BOOST_HANA_STRING("abcd");
        constexpr auto s2 = string_c<'a', 'b', 'c', 'd'>;

        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // make<string_tag>
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<string_tag>(),
            string_c<>
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<string_tag>(char_c<'a'>),
            string_c<'a'>
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<string_tag>(char_c<'a'>, char_c<'b'>),
            string_c<'a', 'b'>
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<string_tag>(char_c<'a'>, char_c<'b'>, char_c<'c'>),
            string_c<'a', 'b', 'c'>
        ));
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<string_tag>(char_c<'a'>, char_c<'b'>, char_c<'c'>, char_c<'d'>),
            string_c<'a', 'b', 'c', 'd'>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make_string(char_c<'a'>, char_c<'b'>, char_c<'c'>),
            make<string_tag>(char_c<'a'>, char_c<'b'>, char_c<'c'>)
        ));
    }

    //////////////////////////////////////////////////////////////////////////
    // to<char const*>
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(is_convertible<string_tag, char const*>{}, "");
        static_assert(!is_embedded<string_tag, char const*>{}, "");

        BOOST_HANA_RUNTIME_CHECK(std::strcmp(
            to<char const*>(BOOST_HANA_STRING("")),
            ""
        ) == 0);

        BOOST_HANA_RUNTIME_CHECK(std::strcmp(
            to<char const*>(BOOST_HANA_STRING("a")),
            "a"
        ) == 0);

        BOOST_HANA_RUNTIME_CHECK(std::strcmp(
            to<char const*>(BOOST_HANA_STRING("ab")),
            "ab"
        ) == 0);

        BOOST_HANA_RUNTIME_CHECK(std::strcmp(
            to<char const*>(BOOST_HANA_STRING("abc")),
            "abc"
        ) == 0);

        BOOST_HANA_RUNTIME_CHECK(std::strcmp(
            to<char const*>(BOOST_HANA_STRING("abcd")),
            "abcd"
        ) == 0);

        // make sure we can turn a non-constexpr hana::string
        // into a constexpr char const*
        auto str = BOOST_HANA_STRING("abcdef");
        constexpr char const* c_str = to<char const*>(str); (void)c_str;
    }

#ifdef BOOST_HANA_CONFIG_ENABLE_STRING_UDL
    //////////////////////////////////////////////////////////////////////////
    // _s user-defined literal
    //////////////////////////////////////////////////////////////////////////
    {
        using namespace boost::hana::literals;

        constexpr auto s1 = "abcd"_s;
        constexpr auto s2 = string_c<'a', 'b', 'c', 'd'>;

        static_assert(std::is_same<decltype(s1), decltype(s2)>::value, "");
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("abcd") == BOOST_HANA_STRING("abcd"));
        BOOST_HANA_CONSTANT_CHECK(BOOST_HANA_STRING("abcd") != BOOST_HANA_STRING("abc"));

        // equal
        BOOST_HANA_CONSTANT_CHECK(equal(
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("abcd")
        ));

        BOOST_HANA_CONSTANT_CHECK(not_(equal(
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("abcde")
        )));

        BOOST_HANA_CONSTANT_CHECK(not_(equal(
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_CHECK(not_(equal(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("abcde")
        )));

        BOOST_HANA_CONSTANT_CHECK(equal(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("")
        ));

        // laws
        auto strings = make<tuple_tag>(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("abcde"),
            BOOST_HANA_STRING("ba")
        );
        test::TestComparable<string_tag>{strings};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING(""), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("a"), f),
                f(char_c<'a'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("ab"), f),
                f(char_c<'a'>, char_c<'b'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abc"), f),
                f(char_c<'a'>, char_c<'b'>, char_c<'c'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abcd"), f),
                f(char_c<'a'>, char_c<'b'>, char_c<'c'>, char_c<'d'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abcde"), f),
                f(char_c<'a'>, char_c<'b'>, char_c<'c'>, char_c<'d'>, char_c<'e'>)
            ));
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("")), size_c<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("a")), size_c<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("ab")), size_c<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("abc")), size_c<3>
            ));
        }

        // laws
        test::TestFoldable<string_tag>{strings};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // front
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                front(BOOST_HANA_STRING("a")),
                char_c<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                front(BOOST_HANA_STRING("ab")),
                char_c<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                front(BOOST_HANA_STRING("abc")),
                char_c<'a'>
            ));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("a")),
                BOOST_HANA_STRING("")
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("ab")),
                BOOST_HANA_STRING("b")
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("abc")),
                BOOST_HANA_STRING("bc")
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(BOOST_HANA_STRING("abcdefghijk")),
                BOOST_HANA_STRING("bcdefghijk")
            ));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(BOOST_HANA_STRING("")));
            BOOST_HANA_CONSTANT_CHECK(is_empty(string_c<>));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("a"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("ab"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("abc"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(string_c<'a'>)));
        }

        // at
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                BOOST_HANA_STRING("abcd")[size_c<2>],
                char_c<'c'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("a"), size_c<0>),
                char_c<'a'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("ab"), size_c<0>),
                char_c<'a'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("abc"), size_c<0>),
                char_c<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("ab"), size_c<1>),
                char_c<'b'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("abc"), size_c<1>),
                char_c<'b'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("abcd"), size_c<1>),
                char_c<'b'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("abc"), size_c<2>),
                char_c<'c'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(BOOST_HANA_STRING("abcd"), size_c<2>),
                char_c<'c'>
            ));
        }

        // laws
        test::TestIterable<string_tag>{strings};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(
            BOOST_HANA_STRING("abc") < BOOST_HANA_STRING("abcd")
        );
        BOOST_HANA_CONSTANT_CHECK(
            BOOST_HANA_STRING("abc") <= BOOST_HANA_STRING("abcd")
        );
        BOOST_HANA_CONSTANT_CHECK(
            BOOST_HANA_STRING("abcd") > BOOST_HANA_STRING("abc")
        );
        BOOST_HANA_CONSTANT_CHECK(
            BOOST_HANA_STRING("abcd") >= BOOST_HANA_STRING("abc")
        );


        // less
        BOOST_HANA_CONSTANT_CHECK(not_(less(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a")
        ));

        BOOST_HANA_CONSTANT_CHECK(not_(less(
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("")
        )));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab")
        ));

        BOOST_HANA_CONSTANT_CHECK(not_(less(
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("ab")
        )));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("abcde")
        ));

        BOOST_HANA_CONSTANT_CHECK(less(
            BOOST_HANA_STRING("abcde"),
            BOOST_HANA_STRING("abfde")
        ));

        // laws
        auto strings = make<tuple_tag>(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("abc"),
            BOOST_HANA_STRING("ba"),
            BOOST_HANA_STRING("abd")
        );
        test::TestOrderable<string_tag>{strings};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        // any_of
        {
            BOOST_HANA_CONSTANT_CHECK(
                any_of(BOOST_HANA_STRING("abcd"), equal.to(char_c<'b'>))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(BOOST_HANA_STRING(""), always(true_c)))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(any_of(BOOST_HANA_STRING("abcd"), equal.to(char_c<'z'>)))
            );
        }

        // find_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(BOOST_HANA_STRING(""), always(true_c)),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(BOOST_HANA_STRING("abcd"), equal.to(char_c<'a'>)),
                just(char_c<'a'>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(BOOST_HANA_STRING("abcd"), equal.to(char_c<'c'>)),
                just(char_c<'c'>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find_if(BOOST_HANA_STRING("abcd"), equal.to(char_c<'d'>)),
                just(char_c<'d'>)
            ));
        }

        // contains
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(
                contains(BOOST_HANA_STRING("abcd"), char_c<'a'>)
            );
            BOOST_HANA_CONSTANT_CHECK(
                contains(BOOST_HANA_STRING("abcd"), char_c<'c'>)
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(contains(BOOST_HANA_STRING("abcd"), char_c<'e'>))
            );
            BOOST_HANA_CONSTANT_CHECK(
                not_(contains(BOOST_HANA_STRING("abcd"), invalid{}))
            );
        }

        // find
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), char_c<'a'>),
                just(char_c<'a'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), char_c<'c'>),
                just(char_c<'c'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), invalid{}),
                nothing
            ));
        }

        // laws
        auto keys = tuple_c<char, 'a', 'f'>;
        auto strings = make<tuple_tag>(
            BOOST_HANA_STRING(""),
            BOOST_HANA_STRING("a"),
            BOOST_HANA_STRING("ab"),
            BOOST_HANA_STRING("abcd"),
            BOOST_HANA_STRING("ba"),
            BOOST_HANA_STRING("afcd")
        );
        test::TestSearchable<string_tag>{strings, keys};
    }
#endif
}
