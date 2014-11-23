/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */


#include <boost/hana/functional/always.hpp>

#include <test/tracked.hpp>

#include <utility>
using namespace boost::hana;


template <typename T>
decltype(auto) call_always(T&& x) {
    return always(std::forward<T>(x))();
}

int main() {
    auto copy = call_always(test::Tracked{1});
    (void)copy;
}
