/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/comparable/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


struct T; struct U;

int main() {
    BOOST_HANA_STATIC_ASSERT(Comparable::laws::check(
        list(type<T>, type<U>, type<void>, type<int>, type<int&>)
    ));
}
