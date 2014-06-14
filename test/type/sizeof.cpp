/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    struct X { };
    BOOST_HANA_STATIC_ASSERT(sizeof_(type<int>) == sizeof(int));

    BOOST_HANA_STATIC_ASSERT(sizeof_(type<X>) == sizeof(X));
    BOOST_HANA_STATIC_ASSERT(sizeof_(type<X>) == sizeof(X{}));
}
