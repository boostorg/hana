/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/searchable.hpp>
using namespace boost::hana;


// A simple infinite Iterable.
template <int i>
struct counter { };

namespace boost { namespace hana {
    template <int i>
    struct head_impl<counter<i>> {
        static constexpr auto apply(counter<i>) { return int_<i>; }
    };

    template <int i>
    struct tail_impl<counter<i>> {
        static constexpr auto apply(counter<i>) { return counter<i + 1>{}; }
    };

    template <int i>
    struct is_empty_impl<counter<i>> {
        static constexpr auto apply(counter<i>) { return false_; }
    };


    template <int i>
    struct any_of_impl<counter<i>> : Iterable::any_of_impl<counter<i>> { };

    template <int i>
    struct find_if_impl<counter<i>> : Iterable::find_if_impl<counter<i>> { };
}}


int main() {
    // Even though the Iterable is infinite, the Searchable instances should
    // stop because they should short-circuit their search.
    BOOST_HANA_CONSTANT_CHECK(any_of(counter<1>{}, equal.to(int_<4>)));

    BOOST_HANA_CONSTANT_CHECK(equal(
        find_if(counter<1>{}, equal.to(int_<4>)),
        just(int_<4>)
    ));
}
