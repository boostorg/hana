/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/integer_sequence.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/iterable.hpp>
#include <laws/searchable.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>
using namespace boost::hana;


using T = int;
using U = long long;

int main() {
    auto sequences = make<tuple_tag>(
        std::integer_sequence<T>{},
        std::integer_sequence<U>{},

        std::integer_sequence<T, 0>{},
        std::integer_sequence<U, 0>{},

        std::integer_sequence<T, 0, 1>{},
        std::integer_sequence<U, 0, 1>{},

        std::integer_sequence<T, 1, 0, 2>{},
        std::integer_sequence<U, 1, 0, 2>{}
    );
    (void)sequences;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                std::integer_sequence<T>{},
                std::integer_sequence<U>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::integer_sequence<T, 0>{},
                std::integer_sequence<U>{}
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::integer_sequence<T>{},
                std::integer_sequence<U, 0>{}
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                std::integer_sequence<T, 0>{},
                std::integer_sequence<U, 0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::integer_sequence<T, 0>{},
                std::integer_sequence<U, 0, 1>{}
            )));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::integer_sequence<T, 0, 2>{},
                std::integer_sequence<U, 0, 1>{}
            )));

            BOOST_HANA_CONSTANT_CHECK(equal(
                std::integer_sequence<T, 0, 1, 2, 3>{},
                std::integer_sequence<U, 0, 1, 2, 3>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(
                std::integer_sequence<T, 0, 1, 2, 3, 5>{},
                std::integer_sequence<U, 0, 1, 2, 3>{}
            )));
        }

        // laws
        test::TestComparable<ext::std::integer_sequence_tag>{sequences};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(std::integer_sequence<T>{}, f),
                f()
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(std::integer_sequence<T, 0>{}, f),
                f(std::integral_constant<T, 0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(std::integer_sequence<T, 0, 1>{}, f),
                f(std::integral_constant<T, 0>{}, std::integral_constant<T, 1>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(std::integer_sequence<T, 0, 1, 2>{}, f),
                f(std::integral_constant<T, 0>{}, std::integral_constant<T, 1>{},
                  std::integral_constant<T, 2>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(std::integer_sequence<T, 0, 1, 2, 3>{}, f),
                f(std::integral_constant<T, 0>{}, std::integral_constant<T, 1>{},
                  std::integral_constant<T, 2>{}, std::integral_constant<T, 3>{})
            ));
        }

        // laws
        test::TestFoldable<ext::std::integer_sequence_tag>{sequences};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // front
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                front(std::index_sequence<0>{}),
                std::integral_constant<std::size_t, 0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                front(std::index_sequence<0, 1>{}),
                std::integral_constant<std::size_t, 0>{}
            ));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(std::index_sequence<>{}));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(std::index_sequence<0>{})));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(std::index_sequence<1>{})));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(std::index_sequence<0>{}),
                std::index_sequence<>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(std::index_sequence<0, 1>{}),
                std::index_sequence<1>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(std::index_sequence<0, 1, 2>{}),
                std::index_sequence<1, 2>{}
            ));
        }

        // laws
        test::TestIterable<ext::std::integer_sequence_tag>{sequences};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        auto sequences = make<tuple_tag>(
            std::integer_sequence<T>{},
            std::integer_sequence<T, 2>{},
            std::integer_sequence<T, 3>{},
            std::integer_sequence<T, 3, 4>{},
            std::integer_sequence<T, 3, 4, 5>{}
        );

        auto keys = make<tuple_tag>(
            std::integral_constant<T, 3>{}, std::integral_constant<U, 4>{}
        );

        // laws
        test::TestSearchable<ext::std::integer_sequence_tag>{sequences, keys};
    }
#endif
}
