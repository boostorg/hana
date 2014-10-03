/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


int main() {
    //! [main]
    auto foo = make<std::string>("foo");
    BOOST_HANA_RUNTIME_ASSERT(foo == "foo");

    auto ts = make<Tuple>(1, '2', 3.3, std::string{"abcd"});
    BOOST_HANA_RUNTIME_ASSERT(ts == tuple(1, '2', 3.3, std::string{"abcd"}));
    //! [main]
}
