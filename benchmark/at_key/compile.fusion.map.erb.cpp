/*
@copyright Jason Rice 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/include/pair.hpp>
namespace fusion = boost::fusion;


template <int>
struct x { };

struct undefined { };

int main() {
    constexpr auto map = fusion::make_map<
        <%= (0...input_size).map { |n| "x<#{n}>" }.join(', ') %>
    >(
        <%= (0...input_size).map { |n| "undefined{}" }.join(', ') %>
    );
    constexpr auto result = fusion::at_key<x<<%= input_size-1 %>>>(map);
    (void)result;
}
