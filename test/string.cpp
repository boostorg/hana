/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/orderable.hpp>
#include <test/auto/searchable.hpp>

#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<String> = make<Tuple>(
          type<Comparable>
        , type<Orderable>

        , type<Foldable>
        , type<Iterable>
        , type<Searchable>
    );

    template <>
    auto objects<String> = make<Tuple>(
        BOOST_HANA_STRING(""),
        BOOST_HANA_STRING("a"),
        BOOST_HANA_STRING("ab"),
        BOOST_HANA_STRING("abc"),
        BOOST_HANA_STRING("abcd"),
        BOOST_HANA_STRING("abcde"),
        BOOST_HANA_STRING("ba"),
        BOOST_HANA_STRING("afcd")
    );
}}}


int main() {
    test::check_datatype<String>();

    // string and BOOST_HANA_STRING
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto const s1 = BOOST_HANA_STRING("abcd");
        constexpr auto s2 = string<'a', 'b', 'c', 'd'>;

        static_assert(std::is_same<
            decltype(s1), decltype(s2)
        >::value, "");
    }

    // Comparable
    {
        // equal
        {
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
        }

        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(
                BOOST_HANA_STRING("abc") == BOOST_HANA_STRING("abc")
            );

            BOOST_HANA_CONSTANT_CHECK(
                BOOST_HANA_STRING("abc") != BOOST_HANA_STRING("fgh")
            );
        }
    }

    // Orderable
    {
        // less
        {
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
        }

        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(
                BOOST_HANA_STRING("abc") < BOOST_HANA_STRING("abcde")
            );

            BOOST_HANA_CONSTANT_CHECK(
                BOOST_HANA_STRING("abd") > BOOST_HANA_STRING("abc")
            );

            BOOST_HANA_CONSTANT_CHECK(
                BOOST_HANA_STRING("abc") <= BOOST_HANA_STRING("abc")
            );

            BOOST_HANA_CONSTANT_CHECK(
                BOOST_HANA_STRING("abc") >= BOOST_HANA_STRING("abc")
            );
        }
    }

    // Foldable
    {
        auto f = test::injection([]{});

        // unpack
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING(""), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("a"), f),
                f(char_<'a'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("ab"), f),
                f(char_<'a'>, char_<'b'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abc"), f),
                f(char_<'a'>, char_<'b'>, char_<'c'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abcd"), f),
                f(char_<'a'>, char_<'b'>, char_<'c'>, char_<'d'>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(BOOST_HANA_STRING("abcde"), f),
                f(char_<'a'>, char_<'b'>, char_<'c'>, char_<'d'>, char_<'e'>)
            ));
        }

        // length
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("")), size_t<0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("a")), size_t<1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("ab")), size_t<2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                length(BOOST_HANA_STRING("abc")), size_t<3>
            ));
        }
    }

    // Iterable
    {
        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(BOOST_HANA_STRING("a")),
                char_<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                head(BOOST_HANA_STRING("ab")),
                char_<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                head(BOOST_HANA_STRING("abc")),
                char_<'a'>
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
            BOOST_HANA_CONSTANT_CHECK(is_empty(string<>));

            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("a"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("ab"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(BOOST_HANA_STRING("abc"))));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(string<'a'>)));
        }

        // at
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, BOOST_HANA_STRING("a")),
                char_<'a'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, BOOST_HANA_STRING("ab")),
                char_<'a'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<0>, BOOST_HANA_STRING("abc")),
                char_<'a'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, BOOST_HANA_STRING("ab")),
                char_<'b'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, BOOST_HANA_STRING("abc")),
                char_<'b'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<1>, BOOST_HANA_STRING("abcd")),
                char_<'b'>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<2>, BOOST_HANA_STRING("abc")),
                char_<'c'>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                at(int_<2>, BOOST_HANA_STRING("abcd")),
                char_<'c'>
            ));
        }

        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                BOOST_HANA_STRING("abcde")[int_<2>],
                char_<'c'>
            ));
        }
    }

    // Searchable
    {
        // any
        {
            BOOST_HANA_CONSTANT_CHECK(
                any(BOOST_HANA_STRING("abcd"), [](auto c) {
                    return equal(c, char_<'b'>);
                })
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(any(BOOST_HANA_STRING(""), [](auto c) {
                    return true_;
                }))
            );

            BOOST_HANA_CONSTANT_CHECK(
                not_(any(BOOST_HANA_STRING("abcd"), [](auto c) {
                    return equal(c, char_<'z'>);
                }))
            );
        }

        // find
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING(""), [](auto c) { return true_; }),
                nothing
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), [](auto c) {
                    return equal(c, char_<'a'>);
                }),
                just(char_<'a'>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), [](auto c) {
                    return equal(c, char_<'c'>);
                }),
                just(char_<'c'>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(BOOST_HANA_STRING("abcd"), [](auto c) {
                    return equal(c, char_<'d'>);
                }),
                just(char_<'d'>)
            ));
        }

        // elem
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(elem(BOOST_HANA_STRING("abcd"), char_<'a'>));
            BOOST_HANA_CONSTANT_CHECK(elem(BOOST_HANA_STRING("abcd"), char_<'c'>));
            BOOST_HANA_CONSTANT_CHECK(not_(elem(BOOST_HANA_STRING("abcd"), char_<'e'>)));
            BOOST_HANA_CONSTANT_CHECK(not_(elem(BOOST_HANA_STRING("abcd"), invalid{})));
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
    }
}
