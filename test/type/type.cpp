/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


int main() {
    struct T { };

    // decltype_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(decltype_(1), type<int>));
        BOOST_HANA_CONSTANT_ASSERT(equal(decltype_('1'), type<char>));
        BOOST_HANA_CONSTANT_ASSERT(equal(decltype_(T{}), type<T>));
    }

    // sizeof_
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(sizeof_(type<int>), size_t<sizeof(int)>));
        BOOST_HANA_CONSTANT_ASSERT(equal(sizeof_(type<T>), size_t<sizeof(T)>));
    }
}
