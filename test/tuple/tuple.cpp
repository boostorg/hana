/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <utility>
using namespace boost::hana;


struct movable {
    movable() = default;
    movable(movable const&) = delete;
    movable(movable&&) = default;
};

int main() {
    // disabled until c++17; see the comment on the definition of hana::tuple
    // for details
#if 0
    // movable only
    {
        auto x = tuple(movable{});
        auto z = std::move(x);
    }
#endif
}
