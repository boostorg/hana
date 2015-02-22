/*
@copyright Zach Laine 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include "benchmark.hpp"
#include <cstdlib>
namespace bh = boost::hana;


int main () {
    boost::hana::benchmark::measure([] {
        long double result = 0;
        for (int iteration = 0; iteration < 1 << 10; ++iteration) {
            auto values = bh::make<bh::Tuple>(
                <%=
                    types.map { |t| "static_cast<#{t}>(std::rand())" }.join(', ')
                %>
            );

            result += bh::foldl(values, 0, [](auto state, auto t) {
                return state + t;
            });
        }
    });
}
