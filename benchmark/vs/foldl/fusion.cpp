/*
@copyright Zach Laine 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/container/vector/vector10.hpp>
#include <boost/fusion/container/vector/vector20.hpp>

#include "benchmark.hpp"
#include <cstdlib>
namespace bf = boost::fusion;


int main () {
    using vector_type = bf::vector<%=input_size%><
        <%= types.join(', ') %>
    >;

    boost::hana::benchmark::measure([] {
        long double result = 0;
        for (int iteration = 0; iteration < 1 << 10; ++iteration) {
            vector_type values(
                <%=
                    types.map { |t| "static_cast<#{t}>(std::rand())" }.join(', ')
                %>
            );

            result += bf::fold(values, 0, [](auto state, auto t) {
                return state + t;
            });
        }
    });
}
