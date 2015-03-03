/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integer_sequence.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>

// instances
#include <test/auto/comparable.hpp>
#include <test/auto/iterable.hpp>

#include <cstddef>
#include <utility>
using namespace boost::hana;


using T = int;
using U = long long;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::std::IntegerSequence> = make<Tuple>(
        type<Iterable>,
        type<Comparable>
    );

    template <>
    auto objects<ext::std::IntegerSequence> = make<Tuple>(
        ::std::integer_sequence<T>{},
        ::std::integer_sequence<U>{},

        ::std::integer_sequence<T, 0>{},
        ::std::integer_sequence<U, 0>{},

        ::std::integer_sequence<T, 0, 1>{},
        ::std::integer_sequence<U, 0, 1>{},

        ::std::integer_sequence<T, 1, 0, 2>{},
        ::std::integer_sequence<U, 1, 0, 2>{}
    );
}}}


int main() {
    test::check_datatype<ext::std::IntegerSequence>();

    // Comparable
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
    }

    // Iterable
    {
        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(std::index_sequence<0>{}),
                std::integral_constant<std::size_t, 0>{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                head(std::index_sequence<0, 1>{}),
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
    }
}
