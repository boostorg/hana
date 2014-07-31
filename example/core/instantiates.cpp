/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/instantiates.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(instantiates<Foldable, Maybe>);
    BOOST_HANA_STATIC_ASSERT(!instantiates<Logical, Maybe>);
    BOOST_HANA_STATIC_ASSERT(!instantiates<Foldable, int>);
    //! [main]
}
