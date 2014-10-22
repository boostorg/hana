/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define BOOST_PP_VARIADICS 1
#include <boost/hana/foldable.hpp>
#include <boost/hana/record_macros.hpp>

#include <utility>
using namespace boost::hana;


struct moveonly {
    moveonly() = default;
    moveonly(moveonly const&) = delete;
    moveonly(moveonly&&) = default;
};

struct T {
    BOOST_HANA_DEFINE_RECORD_INTRUSIVE(T,
        (moveonly, member1),
        (moveonly, member2)
    );
};

int f(int, moveonly) { return 1; }
int g(moveonly, int) { return 1; }

int main() {
    // Folding a Record with move-only members should work when the
    // Record is an rvalue.
    foldl(T{}, 0, f);
    foldr(T{}, 0, g);
}
