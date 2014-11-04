/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(or_(false_, false_, true_));
    BOOST_HANA_CONSTANT_ASSERT(!or_(false_, false_, false_));
    //! [main]

    // Not supported right now.
    // struct invalid { };
    // BOOST_HANA_STATIC_ASSERT(or_(true_, invalid{}));
}
