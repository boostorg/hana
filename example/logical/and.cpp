/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTANT_ASSERT(and_(true_, true_, true_, true_));
    BOOST_HANA_CONSTEXPR_ASSERT(!and_(true_, false, true_, true_));
    //! [main]

    // Not supported right now.
    // struct invalid { };
    // BOOST_HANA_STATIC_ASSERT(!and_(false_, invalid{}));
}
