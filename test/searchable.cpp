/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/searchable.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/placeholder.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>

#include <cstddef>
using namespace boost::hana;


template <typename T, std::size_t n>
using array = T[n];

int main() {
    // Model of Searchable for builtin arrays
    {
        // any_of
        {
            static_assert(
                not_(any_of(array<int, 1>{0}, _ == 1))
            , "");

            static_assert(
                any_of(array<int, 2>{0, 1}, _ == 0)
            , "");
            static_assert(
                any_of(array<int, 2>{0, 1}, _ == 1)
            , "");
            static_assert(
                not_(any_of(array<int, 2>{0, 1}, _ == 2))
            , "");

            static_assert(
                any_of(array<int, 3>{0, 1, 2}, _ == 0)
            , "");
            static_assert(
                any_of(array<int, 3>{0, 1, 2}, _ == 1)
            , "");
            static_assert(
                any_of(array<int, 3>{0, 1, 2}, _ == 2)
            , "");
            static_assert(
                not_(any_of(array<int, 3>{0, 1, 2}, _ == 3))
            , "");
        }

        // find
        // Note: Because we need the predicate to return a Constant, this
        // is incredibly not powerful.
        {
            static_assert(equal(
                find(array<int, 1>{0}, always(true_)),
                just(0)
            ), "");

            BOOST_HANA_CONSTANT_CHECK(equal(
                find(array<int, 1>{0}, always(false_)),
                nothing
            ));
        }
    }
}
