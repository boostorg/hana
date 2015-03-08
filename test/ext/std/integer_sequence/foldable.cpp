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
#include <laws/foldable.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


using T = int;
using U = long long;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto sequences = make<Tuple>(
        std::integer_sequence<T>{},
        std::integer_sequence<U>{},

        std::integer_sequence<T, 0>{},
        std::integer_sequence<U, 0>{},

        std::integer_sequence<T, 0, 1>{},
        std::integer_sequence<U, 0, 1>{},

        std::integer_sequence<T, 1, 0, 2>{},
        std::integer_sequence<U, 1, 0, 2>{}
    );

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
        test::TestFoldable<ext::std::IntegerSequence>{sequences};
    }
}
