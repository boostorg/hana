/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_STATIC_ASSERT(and_());
    BOOST_HANA_STATIC_ASSERT(and_(true_, true_, true_, true_));
    // BOOST_HANA_STATIC_ASSERT(!and_(true_, false_, "never evaluated"));
    //! [main]
}
