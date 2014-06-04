/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/trait.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto xs = list_t<void, int(), char[10]>;
    BOOST_HANA_STATIC_ASSERT(fmap(trait::add_pointer, xs) == list_t<void*, int(*)(), char(*)[10]>);
    BOOST_HANA_STATIC_ASSERT(head(fmap(trait::add_pointer, xs)) == type<void*>);
}
