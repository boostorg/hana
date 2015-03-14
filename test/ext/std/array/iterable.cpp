/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/foldable.hpp>
#include <laws/iterable.hpp>

#include <array>
using namespace boost::hana;


template <int ...i>
constexpr auto array = std::array<int, sizeof...(i)>{{i...}};

int main() {
    auto int_arrays = make<Tuple>(
          array<>
        , array<0>
        , array<0, 1>
        , array<0, 1, 2>
        , array<0, 1, 2, 3>
        , array<0, 1, 2, 3, 4>
    );

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    test::TestFoldable<ext::std::Array>{int_arrays};

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(array<>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(array<0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(array<0, 1>)));
        }

        // head
        {
            BOOST_HANA_CONSTEXPR_CHECK(head(array<0>) == 0);
            BOOST_HANA_CONSTEXPR_CHECK(head(array<0, 1>) == 0);
            BOOST_HANA_CONSTEXPR_CHECK(head(array<0, 1, 2>) == 0);
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(array<0>),
                array<>
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                tail(array<0, 1>),
                array<1>
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                tail(array<0, 1, 2>),
                array<1, 2>
            ));
            BOOST_HANA_CONSTEXPR_CHECK(equal(
                tail(array<0, 1, 2, 3>),
                array<1, 2, 3>
            ));
        }

        // laws
        test::TestIterable<ext::std::Array>{int_arrays};
    }
}
