/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/array.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>

// instances
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>

#include <array>
using namespace boost::hana;


template <int ...i>
constexpr auto array = std::array<int, sizeof...(i)>{{i...}};

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::std::Array> = tuple(
        //! @todo Array is not actually a List, because it can only hold
        //! homogeneous objects.
#if 0
        type<List>,
#endif
        type<Iterable>
    );

    template <>
    auto objects<ext::std::Array> = tuple(
        ::array<>,
        ::array<0>,
        ::array<0, 1>,
        ::array<0, 1, 2>
    );
}}}


int main() {
    test::check_datatype<ext::std::Array>();

    // Iterable
    {
        // is_empty
        {
            BOOST_HANA_CONSTANT_ASSERT(is_empty(array<>));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(array<0>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(is_empty(array<0, 1>)));
        }

        // head
        {
            BOOST_HANA_CONSTEXPR_ASSERT(head(array<0>) == 0);
            BOOST_HANA_CONSTEXPR_ASSERT(head(array<0, 1>) == 0);
            BOOST_HANA_CONSTEXPR_ASSERT(head(array<0, 1, 2>) == 0);
        }

        // tail
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(tail(array<0>), array<>));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(tail(array<0, 1>), array<1>));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(tail(array<0, 1, 2>), array<1, 2>));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(tail(array<0, 1, 2, 3>), array<1, 2, 3>));
        }
    }

    // List
    {
        // nil
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(nil<ext::std::Array>, std::array<char, 0>{}));
            BOOST_HANA_CONSTANT_ASSERT(equal(nil<ext::std::Array>, std::array<int, 0>{}));
            BOOST_HANA_CONSTANT_ASSERT(equal(nil<ext::std::Array>, std::array<long, 0>{}));
        }

        // cons
        {
            BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<>), array<0>));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<1>), array<0, 1>));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<1, 2>), array<0, 1, 2>));
            BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, array<1, 2, 3>), array<0, 1, 2, 3>));

            BOOST_HANA_CONSTEXPR_ASSERT(equal(cons(0, nil<ext::std::Array>), array<0>));
        }
    }
}
