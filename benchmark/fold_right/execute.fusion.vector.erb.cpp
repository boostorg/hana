/*
@copyright Louis Dionne 2015
@copyright Zach Laine 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

<% if input_size > 10 %>
    #define FUSION_MAX_VECTOR_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/reverse_fold.hpp>

#include "measure.hpp"
#include <cstdlib>


int main () {
    boost::hana::benchmark::measure([] {
        long double result = 0;
        for (int iteration = 0; iteration < 1 << 10; ++iteration) {
            auto values = boost::fusion::make_vector(
                <%= input_size.times.map { 'std::rand()' }.join(', ') %>
            );

            result += boost::fusion::reverse_fold(values, 0, [](auto state, auto t) {
                return state + t;
            });
        }
    });
}
