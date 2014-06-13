/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
using namespace boost::hana;


//! [main]
template <typename T>
constexpr auto f(T t) {
    return [=](auto ...xs) {
        return f(always(t)(xs...));

        // This would hit the recursive template instantiation limit
        // while trying to instantiate f<T> too soon.
        // return f(t);
    };
}

int main() {
    f(1);
}
//! [main]
