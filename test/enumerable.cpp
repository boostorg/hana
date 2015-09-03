/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/concept/enumerable.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/enumerable.hpp>
using namespace boost::hana;


int main() {
    test::TestEnumerable<int>{make<tuple_tag>(0,1,2,3,4,5)};
    test::TestEnumerable<unsigned int>{make<tuple_tag>(0u,1u,2u,3u,4u,5u)};
    test::TestEnumerable<long>{make<tuple_tag>(0l,1l,2l,3l,4l,5l)};
    test::TestEnumerable<unsigned long>{make<tuple_tag>(0ul,1ul,2ul,3ul,4ul,5ul)};

    // succ
    static_assert(succ(6) == 7, "");
    static_assert(succ(0) == 1, "");
    static_assert(succ(-3) == -2, "");

    // pred
    static_assert(pred(6) == 5, "");
    static_assert(pred(0) == -1, "");
    static_assert(pred(-3) == -4, "");
}
