/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/functional/id.hpp>

#include <iostream>
using namespace boost::hana;


struct tracked {
    int i;

    explicit tracked(int k) : i{k} {
        std::cout << "constructing tracked{" << i << "}\n";
    }

    tracked(tracked const& t) : i{t.i + 1} {
        std::cout << "copying tracked{" << t.i << "} to tracked{" << i << "}\n";
    }

    tracked(tracked&& t) : i{t.i + 1} {
        std::cout << "moving tracked{" << t.i << "} to tracked{" << i << "}\n";
        t.i = -t.i;
    }

    ~tracked() {
        std::cout << "destructing tracked{" << i << "}\n";
        BOOST_HANA_RUNTIME_CHECK(i == 1 || i == 2 || i == -1 || i == -2);
    }
};

decltype(auto) f() {
    return id(tracked{1});
}

int main() {
    auto z = f();
    (void)z;
}
