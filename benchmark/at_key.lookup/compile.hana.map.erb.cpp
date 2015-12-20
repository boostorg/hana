/*
@copyright Jason Rice 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at_key.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/map.hpp>
namespace hana = boost::hana;


template <int>
struct x { };

struct undefined {};

int main() {
    constexpr auto map = hana::make_map(
        <%= (0...200).map { |n| "hana::make_pair(hana::type_c<x<#{n}>>, undefined{})" }
          .join(', ') %>
    );
    constexpr auto result = hana::at_key(map, hana::type_c<x<<%= input_size-1 %>>>);
    (void)result;
}
