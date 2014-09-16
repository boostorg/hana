/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/tuple.hpp>

#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    {
        //! [fmap]
        auto to_string = [](auto x) {
            return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
        };

        BOOST_HANA_RUNTIME_ASSERT(
            fmap(tuple(1, '2', "345", std::string{"67"}), to_string) ==
            tuple("1", "2", "345", "67")
        );
        //! [fmap]
    }
}
