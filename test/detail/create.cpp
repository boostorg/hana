/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/create.hpp>

#include <utility>
#include <tuple>
using namespace boost::hana;


constexpr detail::create<std::tuple> make_tuple{};
constexpr detail::create<std::pair> make_pair{};

int main() {
    static_assert(make_tuple(1, '2', 3.3) == std::make_tuple(1, '2', 3.3), "");
    static_assert(make_pair(1, '2') == std::make_pair(1, '2'), "");
}
