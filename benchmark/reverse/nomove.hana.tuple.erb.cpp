/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/reverse.hpp>
#include <boost/hana/tuple.hpp>

#include "measure.hpp"
#include <cstdlib>
#include <string>
#include <utility>


int main () {
    std::string s(1000, 'x');
    boost::hana::benchmark::measure([&] {
        for (int iteration = 0; iteration < 1 << 5; ++iteration) {
            auto values = boost::hana::make_tuple(
                <%= input_size.times.map { 's' }.join(', ') %>
            );

            auto result = boost::hana::reverse(values);
            (void)result;
        }
    });
}
